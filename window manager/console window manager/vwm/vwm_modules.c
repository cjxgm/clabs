#include <dirent.h>
#include <string.h>

#include <sys/types.h>

#include <gmodule.h>
#include <pseudo.h>

#include "vwm.h"
#include "vwm_modules.h"
#include "vwm_profile.h"

gchar* vwm_modules_load(gchar *module_dir)
{
	VWM_MODULE		*vwm_module;
	gchar				app_path[NAME_MAX];
	DIR				*search_dir=NULL;
	struct dirent	*dirent_file=NULL;
	GSList			*module_list=NULL;
	GSList			*node;
   GModule        *module;
   gchar          *buffer=NULL;
   gchar          *error_msg=NULL;

   if(module_dir==NULL) return NULL;
	search_dir=opendir(module_dir);
   if(search_dir==NULL)
   {
      buffer=g_strdup_printf("error opening module directory:\n%s",module_dir);
      return buffer;
   }

	module_list=vwm_modules_list(NULL);

	while((dirent_file=readdir(search_dir))!=NULL)
	{
		if(strcmp(dirent_file->d_name,".")==0) continue;
		if(strcmp(dirent_file->d_name,"..")==0) continue;

		/*	fix module file name (maybe not necessary)	*/
		if(module_dir[strlen(module_dir)-1]=='/')
			sprintf(app_path,"%s%s",module_dir,dirent_file->d_name);
		else
			sprintf(app_path,"%s/%s",module_dir,dirent_file->d_name);

		/*	check to see if module is already registered	*/
		node=module_list;
		while(node!=NULL)
		{
			vwm_module=(VWM_MODULE*)node->data;
			if(g_strrstr_len(app_path,strlen(app_path),
				vwm_module->libfilename)!=NULL) break;
			node=node->next;
		}

		/*	load module	*/
		if(node==NULL)
      {
         module=g_module_open(app_path,
            G_MODULE_BIND_LAZY | G_MODULE_BIND_LOCAL);
         if(module==NULL)
         {
            error_msg=(gchar*)g_module_error();
            if(error_msg!=NULL) buffer=g_strdup_printf("%s\n%s\n\n%s",
               "Could not register module:",app_path,error_msg);
            else buffer=g_strdup_printf("%s\n%s\n",
               "Could not register module:",app_path);
         }
      }
	}
   closedir(search_dir);

	/*	obligatory clean up	*/
	if(module_list!=NULL) g_slist_free(module_list);
   if(buffer!=NULL) return buffer;

	return NULL;
}

VWM_MODULE* vwm_module_add(gchar *category,gchar *title,
	VWM_MOD_MAIN vwm_mod_main,gpointer anything,gchar *libfilename)
{
	VWM			*vwm;
	VWM_MODULE	*vwm_module;
	GSList		*node;

	if(category==NULL || title==NULL || vwm_mod_main==NULL || libfilename==NULL)
		return NULL;
	if(strlen(libfilename)>NAME_MAX) return NULL;
	vwm=vwm_get_instance();

	/*	search for existing category name	*/
	node=vwm->category_list;
	while(node!=NULL)
	{
		if(strcmp((gchar*)node->data,category)==0) break;
		node=node->next;
	}

	/*	add a new entry to the category list if it doesn't already exist	*/
	if(node==NULL)
		vwm->category_list=g_slist_append(vwm->category_list,(gpointer)category);

	/*	make sure app is not already loaded	*/
	node=vwm->module_list;
	while(node!=NULL)
	{
		vwm_module=(VWM_MODULE*)node->data;
		if(vwm_module->mod_main==vwm_mod_main) return vwm_module;
		node=node->next;
	}

	/*	add the application to the list	*/
	vwm_module=(VWM_MODULE*)g_malloc0(sizeof(VWM_MODULE));
	vwm_module->category=category;
	vwm_module->title=title;
	vwm_module->mod_main=vwm_mod_main;
	vwm_module->anything=anything;
	strcpy(vwm_module->libfilename,libfilename);
	vwm->module_list=g_slist_prepend(vwm->module_list,(gpointer)vwm_module);

	vwm->category_list=g_slist_sort(vwm->category_list,
		(GCompareFunc)vwm_category_sort);

	return vwm_module;
}

GSList*	vwm_modules_list(gchar *category)
{
	VWM			*vwm;
	VWM_MODULE	*vwm_module;
	GSList		*list_copy=NULL;
	GSList		*node;

	vwm=vwm_get_instance();
	if(g_slist_length(vwm->module_list)==0) return NULL;

	if(category==NULL)
	{
		list_copy=g_slist_copy(vwm->module_list);
		return list_copy;
	}

	node=vwm->module_list;
	while(node!=NULL)
	{
		vwm_module=(VWM_MODULE*)node->data;
		if(strcmp(vwm_module->category,category)==0)
			list_copy=g_slist_prepend(list_copy,node->data);
		node=node->next;
	}

	return list_copy;
}

GSList*	vwm_modules_list_categories(void)
{
	VWM		*vwm;
	GSList	*list_copy;

	vwm=vwm_get_instance();
	list_copy=g_slist_copy(vwm->category_list);
	return list_copy;
}

VWM_MODULE* vwm_module_find(gchar *title)
{
	VWM			*vwm;
	VWM_MODULE	*vwm_module;
	GSList		*node;

	if(title==NULL) return NULL;
	vwm=vwm_get_instance();

	node=vwm->module_list;
	while(node!=NULL)
	{
		vwm_module=(VWM_MODULE*)node->data;
		if(strcmp(title,vwm_module->title)==0) break;
		node=node->next;
	}

	if(node==NULL) return NULL;

	return vwm_module;
}

gint vwm_category_sort(gpointer item1,gpointer item2)
{
	gchar	*data1,*data2;

	data1=(gchar*)item1;
	data2=(gchar*)item2;

	if(strcmp(data1,"VWM")==0) return 1;
	if(data1[0]<data2[0]) return -1;
	if(data1[0]>data2[0]) return 1;

	return 0;
}





