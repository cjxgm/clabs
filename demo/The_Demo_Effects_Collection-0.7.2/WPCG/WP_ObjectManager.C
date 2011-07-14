/* Copyright (C) 2001-2003 W.P. van Paassen - peter@paassen.tmfweb.nl

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include <iostream>
#include <cmath>
#include "WP_Model.h"
#include "WP_Camera.h"
#include "WP_AnimationManager.h"
#include "WP_ObjectManager.h"

namespace WPCG
{
WP_Math* WP_DynamicObject::math = WP_Math::getInstance();

WP_Object::WP_Object(const WP_Matrix3D& _matrix, const string& name):matrix(_matrix),object_name(name), heading(0), pitch(0),roll(0), inFrustum(true)
{
  //determine dir and up vectors, it is assumed that the object is orientated correctly thus facing north (0.0f, 0.0f, -1.0f) and up is (0.0f, 1.0f, 0.0f)
  
  dir = WP_Vector3D(0.0f, 0.0f, -1.0f);
  up = WP_Vector3D(0.0f, 1.0f, 0.0f);
};

void WP_Object::drawOpenGL()  
{ 
    model->drawOpenGL(matrix, this);
}

void WP_Object::print() const
{
  cout << endl << "************************************" << endl;
  cout << "Object Name: " << object_name << endl;
  cout << "Model Name: " << model->model_name << endl;
  cout << "Position: x:" << matrix.data[12] << " y:" << matrix.data[13] << " z:" << matrix.data[14] << endl;
  cout << "Heading: " << heading << " degrees" << endl;
  cout << "Pitch: " << pitch << " degrees" << endl;
  cout << "Roll: " << roll << " degrees" << endl;
}

bool 
WP_Object::isAnimated() const
{
  return WP_AnimationManager::getInstance()->isObjectPresent(this);
}

unsigned short 
WP_Object::getAnimationCategories(string **strings) const
{
  if (isAnimated())
    {
      WP_AnimatedModel* amodel = dynamic_cast<WP_AnimatedModel*>(model);
      if (amodel)
	{
	  unsigned short n = amodel->getNumberCategories();
	  *strings = new string[n];
	  unsigned short i;
	  for (i = 0; i < n; ++i)
	    strings[0][i] = amodel->getCategoryName(i);
	  return n;
	}
    }
  *strings = 0;
  return 0;
}

bool 
WP_Object::setAnimationCategory(const string &category) const
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
  return ani->setCategory(this, category);
}

string
WP_Object::getAnimationCategory() const
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
  return ani->getCategory(this);
}

//********************************************* WP_ObjectManager *******************************************************

const unsigned char WP_ObjectManager::num_internal_models = 1;

WP_ObjectManager* WP_ObjectManager::om_instance = 0;

const string WP_ObjectManager::internal_models[] = {"WP_SQUARE"};

WP_ObjectManager::WP_ObjectManager():cam(WP_Camera::getInstance()), unique(0), number_lights(0), number_collisions(0)
{
  PC.SetFirstContact(true);
  PC.SetTemporalCoherence(true);
  TC.SetFirstContact(true);
  TC.SetFullBoxBoxTest(true);
  TC.SetFullPrimBoxTest(true);
  TC.SetTemporalCoherence(true);
  RC.SetFirstContact(false);
  RC.SetTemporalCoherence(false);
  RC.SetClosestHit(true);
  RC.SetCulling(true);
  RC.SetDestination(&CF);
  RC.ValidateSettings();
  SF.SetFirstContact(true);
  SF.SetTemporalCoherence(false);
  SF.SetClosestHit(false);
  SF.SetCulling(false);
  SF.SetDestination(&CF);
  SF.ValidateSettings();
}

WP_ObjectManager::~WP_ObjectManager()
{
  removeAll();
  om_instance = 0;
}

WP_StaticObject* WP_ObjectManager::createStaticObject(const WP_Matrix3D& matrix, const string& object_name, 
							    const string& model_name)
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();

  WP_StaticObject* sobject;
  WP_Model* model;
try
  {
    cout << "Creating static object " << object_name << " of model " << model_name << " at position: " << matrix.data[12] 
	 << " " << matrix.data[13] << " " << matrix.data[14] << endl;
  
    sobject = new WP_StaticObject(matrix, object_name);
    if (!sobject)
      {
	return (WP_StaticObject*)0;
      }

    //check if it's an internal object

    int i;
    bool internal = false;

    for (i = 0; i < num_internal_models; ++i)
      {
	if (object_name == internal_models[i])
	  {
	    internal = true;
	    sobject->name_id = unique++;
	    model = findInstance(model_name);
	    if (model)
	      {
		model->count++;
		sobject->model = model;
		static_objects.push_back(sobject);
	      }
	    else
	      {
		//FIXME switch for internal model type
		model = new WP_Square(model_name);
		if (!model->init())
		  {
		    throw("");
		  }
		sobject->model = model;
		static_objects.push_back(sobject);
	      }
	    break;
	  }
      }
      
    if (!internal)
      {
	string m_name = "../MODELS/" + model_name;
	sobject->name_id = unique++;
	model = findInstance(m_name);
	if (model)
	  {
	    model->count++;
	    sobject->model = model;
	    static_objects.push_back(sobject);
	    if (dynamic_cast<WP_AnimatedModel*>(model))
	      //add to animationmanager  
	      ani->addObject(sobject);
	  }
	else
	  {
	    if (hasValidExtension(m_name, "MD2"))
	      {
		//quake2 md2 file
		model = new WP_Model_MD2(m_name);
		if (!model->init())
		  {
		    throw("");
		  }
		sobject->model = model;
		static_objects.push_back(sobject);
		//add to animationmanager  
		ani->addObject(sobject);
	      }
	    else
	      {
		//other formats like for example lightwave format
		cerr << "ERROR: Unable to load model " << m_name << "only quake2 MD2 file format supported at the moment" << endl;
		throw("");
	      }
 	  }
      }
  }
catch(...)
  {
    delete sobject;
    delete model;
    return (WP_StaticObject*)0;
  }
 return sobject;
}

WP_DynamicObject* WP_ObjectManager::createDynamicObject(const WP_Matrix3D& matrix, const string& object_name, 
							    const string& model_name, const WP_Vector3D& velocity)
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
  WP_DynamicObject* dobject;
  WP_Model* model;
try
  {

    cout << "Creating dynamic object " << object_name << " of model " << model_name << " at position: " << matrix.data[12] 
	 << " " << matrix.data[13] << " " << matrix.data[14] << " with vector " << velocity.data[0] << " " << velocity.data[1] << " " 
	 << velocity.data[2] << endl;

    dobject = new WP_DynamicObject(matrix, object_name, velocity);
    if (!dobject)
      {
	return (WP_DynamicObject*)0;
      }

    //check if it's an internal object

    int i;
    bool internal = false;

    for (i = 0; i < num_internal_models; ++i)
      {
	if (model_name == internal_models[i])
	  {
	    internal = true;
	    dobject->name_id = unique++;
	    model = findInstance(model_name);
	    if (model)
	      {
		model->count++;
		dobject->model = model;

		dynamic_objects.push_back(dobject);
	      }
	    else
	      {
		model = new WP_Square(model_name);
		if (!model->init())
		  {
		    throw("");
		  }
		dobject->model = model;
		dynamic_objects.push_back(dobject);
	      }
	    break;
	  }
      }

    if (!internal)
      {
	string m_name = "../MODELS/" + model_name;
	dobject->name_id = unique++;
	model = findInstance(m_name);
	
	if (model) 
	  {
	    model->count++;
	    dobject->model = model;
	    dynamic_objects.push_back(dobject);
	    if (dynamic_cast<WP_AnimatedModel*>(model))
	      //add to animationmanager  
	      ani->addObject(dobject);
	  }
	else
	  {
	    if (hasValidExtension(m_name, "MD2"))
	      {
		//quake2 MD2 file
		model = new WP_Model_MD2(m_name);
		if (!model->init())
		  {
		    throw("");
		  }
		dobject->model = model;
		dynamic_objects.push_back(dobject);
		//add to animationmanager  
		ani->addObject(dobject);
	      }
	    else
	      {
		//other formats like for example lightwave format
	      cout << "ERROR: Unable to load model " << m_name << "\nonly quake2 MD2 fileformat supported at the moment" << endl;
	      throw("");
	      }
	  }
      }
  }
catch(...)
  {
    delete dobject;
    delete model;
    return (WP_DynamicObject*)0;
  }
 return dobject;
}

WP_Object* WP_ObjectManager::getObject(const string& name) const
{
  WP_Object* object = getStaticObject(name);
  if (!object)
    {
      return getDynamicObject(name);
    }
  return object;
}

//if last == 0 then the first element of the list is given
//0 is returned when list is empty
WP_StaticObject* WP_ObjectManager::getNextStaticObject(const WP_StaticObject* last) const
{
  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  if (last)
    {
      while (i != static_objects.end())
	{	
	  if ((*i) == last)
	    {
	      if (++i != static_objects.end())
		{
		  return (*i);
		}
	      else
  		  return 0; //empty list
	    }
	  i++;
	}
    }
  else 
	return (*i);
  return 0; //empty list
}

//if last == 0 then the first element of the list is given
//0 is returned when list is empty
WP_DynamicObject* WP_ObjectManager::getNextDynamicObject(const WP_DynamicObject* last) const
{
  list<WP_DynamicObject*>::const_iterator i = dynamic_objects.begin();
  if (last)
    {
      while (i != dynamic_objects.end())
	{	
	  if ((*i) == last)
	    {
	      if (++i != dynamic_objects.end())
		{
		  return (*i);
		}
	     else
  		  return 0; //empty list
	    }
	  i++;
	}
    }
    else
  	return (*i);
    return 0;
}

WP_StaticObject* WP_ObjectManager::getStaticObject(const string& name) const
{
  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
     if ((*i)->object_name == name)
	{
	  return (*i);
	}
      i++;
    }
  return 0; //instance not found
}

WP_StaticObject* WP_ObjectManager::getStaticObject() const
{
  if (static_objects.begin() != static_objects.end())
    {
      list<WP_StaticObject*>::const_iterator i = static_objects.begin();
      return (*i);
    }
  return 0; //no instance available
}

WP_DynamicObject* WP_ObjectManager::getDynamicObject(const string& name) const
{
  list<WP_DynamicObject*>::const_iterator i = dynamic_objects.begin();
  while (i != dynamic_objects.end())
    {	
     if ((*i)->object_name == name)
	{
	  return (*i);
	}
      i++;
    }
  return 0; //instance not found
}

WP_DynamicObject* WP_ObjectManager::getDynamicObject() const
{
  if (dynamic_objects.begin() != dynamic_objects.end())
    {
      list<WP_DynamicObject*>::const_iterator i = dynamic_objects.begin();
      return (*i);
    }
  return 0; //no instance available
}

void 
WP_ObjectManager::checkCollisions()
{
  number_collisions = 0;

  list<WP_CollisionPair*>::const_iterator i = collision_pairs.begin();
  while (i != collision_pairs.end())
    {	
      TC.SetCallback0(&ColCallback, reinterpret_cast<uqword>((*i)->object1));
      TC.SetCallback1(&ColCallback, reinterpret_cast<uqword>((*i)->object2));
   
      if (TC.Collide((*i)->cache, reinterpret_cast<Matrix4x4*>(&(*i)->object1->matrix), 
		     reinterpret_cast<Matrix4x4*>(&(*i)->object2->matrix)))
	{
	  if (TC.GetContactStatus())
	    {
	      (*i)->object1->onCollision();
	      (*i)->object2->onCollision();
	      number_collisions++;
	    }
	}
      else
	cout << "COLLIDING ERROR" << endl;
      i++;
    }
}

bool WP_ObjectManager::inShadow(const WP_Point3D& p, const WP_Point3D& light)
{
  WP_Point3D q = p;
  WP_Vector3D start = cam->eye - p;
  start.normalize();
  q += start * 1.005; //to avoid counting the originating object shift startpoint of ray a little towards the eye
  WP_Vector3D v = light - q;
  SF.SetMaxDist(v.length());
  v.normalize();
  Ray worldray(*(reinterpret_cast<Point*>(&q)),*(reinterpret_cast<const Point*>(&v)));

  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
	      	SF.SetCallback(&ColCallback, uqword(*i));
		if (SF.Collide(worldray,(*i)->model->getCollisionModel(*i),reinterpret_cast<Matrix4x4*>(&(*i)->matrix),NULL))
		{
			//check CF for collisionfaces
			if (CF.GetNbFaces() > 0)
			{
				return true;
			}
		}
	i++;
    }

  list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
	      	SF.SetCallback(&ColCallback, uqword(*j));
		if (SF.Collide(worldray,(*j)->model->getCollisionModel(*j),reinterpret_cast<Matrix4x4*>(&(*j)->matrix),NULL))
		{
			//check CF for collisionfaces
			if (CF.GetNbFaces() > 0)
			{
				return true;
			}
		}
	j++;
    }

  return false;	
			
}

WP_RayHitPoint WP_ObjectManager::castRay(const WP_Ray3D& ray,bool check_frustum) 
{
  Ray worldray(*(reinterpret_cast<const Point*>(&ray.start)),*(reinterpret_cast<const Point*>(&ray.direction)));
  CollisionFace closestFace;
  closestFace.mDistance = cam->maxRay;
  const WP_Object* obj;
  
  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
	if (check_frustum)
		if (!(*i)->inFrustum)
			continue;
	      	RC.SetCallback(&ColCallback, uqword(*i));
		if (RC.Collide(worldray,(*i)->model->getCollisionModel(*i),reinterpret_cast<Matrix4x4*>(&(*i)->matrix),NULL))
		{
			//check CF for collisionfaces
			if (CF.GetNbFaces() > 0)
			{
				if ((*CF.GetFaces()).mDistance < closestFace.mDistance)
				{
					closestFace =  *CF.GetFaces();
					obj = (*i);
				}
			}
		}
	i++;
    }

  list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
	if (check_frustum)
		if (!(*j)->inFrustum)
			continue;
	      	RC.SetCallback(&ColCallback, uqword(*j));
		if (RC.Collide(worldray,(*j)->model->getCollisionModel(*j),reinterpret_cast<Matrix4x4*>(&(*j)->matrix),NULL))
		{
			//check CF for collisionfaces
			if (CF.GetNbFaces() > 0)
			{
				if ((*CF.GetFaces()).mDistance < closestFace.mDistance)
				{
					closestFace =  *CF.GetFaces();
					obj = (*j);
				}
			}
		}
	j++;
    }

    if (closestFace.mDistance < cam->maxRay)
	{
		//find normal of hitpoint

		//get vertices of hit triangle
		WP_Vertex* v1 = obj->model->getVertex(obj,obj->model->triangles[closestFace.mFaceID * 3]);
		WP_Vertex* v2 = obj->model->getVertex(obj,obj->model->triangles[closestFace.mFaceID * 3 + 1]);
		WP_Vertex* v3 = obj->model->getVertex(obj,obj->model->triangles[closestFace.mFaceID * 3 + 2]);

		//interpolate vertex normals to compute normal of hitpoint

		//first interpolate normal from V1 to V3

		WP_Point3D p1 = v1->point + v1->normal;
		WP_Point3D p2 = v3->point + v3->normal;
		WP_Point3D p12 = (v1->point + ((v3->point - v1->point) * closestFace.mV));

		WP_Vector3D norm1 = (p1 + ((p2 - p1) * closestFace.mV)) - p12;
		//norm1.normalize();

		//second interpolate from V3 to V2

		p1 = v3->point + v3->normal;
		p2 = v2->point + v2->normal;

		WP_Point3D p32_0 = p12 + ((v2->point - v1->point) * (1.0f - closestFace.mV));
		scalar l1 = (p32_0 - v3->point).length();
		scalar l2 = (v2->point - v3->point).length();
		scalar ival = (l1 / l2);
		WP_Point3D p32_1 = (p1 + ((p2 - p1) * ival)); 
		WP_Vector3D norm2 = p32_1 - p32_0;
		//norm2.normalize();

		//finaly interpolate between norm1 and norm2
		WP_Vector3D norm3 = ((p12 + norm1) + (((p32_1/*p32_0 + norm2*/) - (p12 + norm1)) * closestFace.mU)) - (p12 + ((p32_0 - p12) * closestFace.mU));
		norm3.normalize();
			
		//multiply normal by inverse transpose of object transformation matrix

		WP_Matrix3D inversetranspose = obj->matrix;
		inversetranspose.inverse();
		inversetranspose.transpose();

		norm3 *= inversetranspose;
		
		//get color from texture if any
		WP_Color color;
		WP_Image* image = WP_TextureManager::getInstance()->getImage(obj->model->tex_id);
		if (image)
		{
			//set correct texture coordinates according to triangle index

			v1->texCoords[0] = obj->model->texs[obj->model->texCoords[closestFace.mFaceID * 3]*2];
			v1->texCoords[1] = obj->model->texs[obj->model->texCoords[closestFace.mFaceID * 3]*2+1];
			v2->texCoords[0] = obj->model->texs[obj->model->texCoords[closestFace.mFaceID * 3+1]*2];
			v2->texCoords[1] = obj->model->texs[obj->model->texCoords[closestFace.mFaceID * 3+1]*2+1];
			v3->texCoords[0] = obj->model->texs[obj->model->texCoords[closestFace.mFaceID * 3+2]*2];
			v3->texCoords[1] = obj->model->texs[obj->model->texCoords[closestFace.mFaceID * 3+2]*2+1];

			//interpolate tex_coords
			scalar coords_p12[2];
			coords_p12[0] = v1->texCoords[0] + ((v3->texCoords[0] - v1->texCoords[0]) * closestFace.mV);
			coords_p12[1] = v1->texCoords[1] - ((v1->texCoords[1] - v3->texCoords[1]) * closestFace.mV);
			coords_p12[0] += ((v2->texCoords[0] - v1->texCoords[0]) * closestFace.mU);
			coords_p12[1] -= ((v1->texCoords[1] - v2->texCoords[1]) * closestFace.mU);
			WP_RGBA* pixel = image->getFastPixel((int)round(coords_p12[0]),(int)round(coords_p12[1]));
			color = WP_Color(pixel->r, pixel->g, pixel->b, pixel->a);
		}
		else
		{
			color = WP_Color(2.0f,2.0f,2.0f,2.0f);
		}
		
	    	return WP_RayHitPoint(norm3,ray.start + ray.direction * closestFace.mDistance,color,obj);

		//WP_Point3D hit = v1->point + ((v2->point - v1->point) * closestFace.mU) + ((v3->point - v1->point) * closestFace.mV);	
		//hit *= obj->matrix;
		//cout << hit.data[0] << " " << hit.data[1] << " " << hit.data[2] << endl;
	}

    return WP_RayHitPoint(WP_Vector3D(),WP_Point3D(), WP_Color(), NULL); 
}

void WP_ObjectManager::drawObjects() 
{
  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
	if ((*i)->inFrustum)
	   (*i)->drawOpenGL();
      	i++;
      
    }

  list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
	if ((*j)->inFrustum)
	   (*j)->drawOpenGL();
        j++;
    }

  //do lighting
  list<WP_Light*>::iterator k = lights.begin();
  while (k != lights.end())
    {	
      (*k)->drawOpenGL();
      k++;
    }
}

void WP_ObjectManager::drawObjectsSelection() 
{
  glInitNames(); //init the name stack

  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      glPushName((*i)->name_id);
      (*i)->drawOpenGL();
      glPopName();
      i++;
    }

  list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      glPushName((*j)->name_id);
      (*j)->drawOpenGL();
      glPopName();
      j++;
    }
}

WP_Model* WP_ObjectManager::findInstance(const string& model_name) 
{
  list<WP_StaticObject*>::iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
       if ((*i)->model->model_name == model_name)
	{
	  return (*i)->model;
	}
      i++;
    }

  list<WP_DynamicObject*>::iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
       if ((*j)->model->model_name == model_name)
	{
	  return (*j)->model;
	}
      j++;
    }
  return 0; //instance not found
}
  
bool WP_ObjectManager::removeStaticObject(const string& name)
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();

  list<WP_StaticObject*>::iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      if ((*i)->object_name == name)
	{
	  (*i)->model->count--;
	  if ((*i)->model->count == 0)
	    {
	      delete (*i)->model;
	    }
	  
	  ani->removeObject(*i);
	  delete (*i);
	  static_objects.erase(i);

	  return true; //object removed
	}
      i++;
    }
  return false; //object not found
}

bool WP_ObjectManager::removeDynamicObject(const string& name)
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
  list<WP_DynamicObject*>::iterator i = dynamic_objects.begin();
  while (i != dynamic_objects.end())
    {	
      if ((*i)->object_name == name)
	{
	  (*i)->model->count--;
	  if ((*i)->model->count == 0)
	    {
	      delete (*i)->model;
	    }
	  
	  ani->removeObject(*i);
	  delete (*i);
	  dynamic_objects.erase(i);

	  return true; //object removed
	}
      i++;
    }
  return false; //object not found
}

bool WP_ObjectManager::removeObject(const string& name)
{
  if (!removeStaticObject(name))
    return removeDynamicObject(name);
  return false;
}

bool WP_ObjectManager::removeSameObjects(const string& name)
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
  int static_counter = countStaticObjects(name);
  int j = 0;
  int static_removed = 0;

  for (; j < static_counter; j++)
    {
       list<WP_StaticObject*>::iterator i = static_objects.begin();
       while (i != static_objects.end())
	 {	
	   if ((*i)->object_name == name)
	     {
	       (*i)->model->count--;
	       if ((*i)->model->count == 0)
		 {
		   delete (*i)->model;
		 }

	       ani->removeObject(*i);
	       delete (*i);
	       static_objects.erase(i);
	       static_removed++;
	       break;
	     }
	   i++;
	 }
    }
  
  int dynamic_counter = countDynamicObjects(name);
  j = 0;
  int dynamic_removed = 0;

  for (; j < dynamic_counter; j++)
    {
       list<WP_DynamicObject*>::iterator i = dynamic_objects.begin();
       while (i != dynamic_objects.end())
	 {	
	   if ((*i)->object_name == name)
	     {
	       (*i)->model->count--;
	       if ((*i)->model->count == 0)
		 {
		   delete (*i)->model;
		 }
	       
	       ani->removeObject(*i);
	       delete (*i);
	       dynamic_objects.erase(i);
	       dynamic_removed++;
	       break;
	     }
	   i++;
	 }
    }
  return static_removed == static_counter && dynamic_removed == dynamic_counter;
}

int WP_ObjectManager::countStaticObjects(const string& name) const
{
  int counter = 0;
  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      if ((*i)->object_name == name)
	{
	  counter++;
	}
      i++;
    }
  return counter;
}

int WP_ObjectManager::countDynamicObjects(const string& name) const
{
  int counter = 0;

  list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      if ((*j)->object_name == name)
	{
	  counter++;
	}
      j++;
    }
  return counter;
}

bool WP_ObjectManager::removeAll()
{
  WP_AnimationManager *ani = WP_AnimationManager::getInstance();

  int count = static_objects.size() + dynamic_objects.size();
  int removed = 0;

  list<WP_StaticObject*>::iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      (*i)->model->count--;
      if ((*i)->model->count == 0)
	{
	  delete (*i)->model;
	}

      ani->removeObject(*i);
      delete (*i);
      removed++;
      i++;
    }
  static_objects.clear();

 list<WP_DynamicObject*>::iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      (*j)->model->count--;
      if ((*j)->model->count == 0)
	{
	  delete (*j)->model;
	}
      
      ani->removeObject(*j);
      delete (*j);
      removed++;
      j++;
    }
  dynamic_objects.clear();

 list<WP_CollisionPair*>::iterator k = collision_pairs.begin();
 while (k != collision_pairs.end())
   {
     delete (*k);
     ++k;
   }
 collision_pairs.clear();

 list<WP_Light*>::const_iterator l = lights.begin();
 while (l != lights.end())
   {
     delete (*l);
     ++l;
   }
 lights.clear();

  return removed == count;
}

WP_Object* WP_ObjectManager::pickObject(int x, int y)
{
  GLuint selectBuf[512];
  WP_GLState* state = WP_GLState::getInstance();
  GLuint hits = 0;

  glSelectBuffer(512, selectBuf);

  state->selection_mode();

  cam->setPickingVolume(5, 5, x, y);

  drawObjectsSelection();

  cam->setRenderVolume();
  hits = state->render_mode();

  //now all objects are rendered to the selection buffer and a hitlist was created and stored in selectBuf. The picked object name is stored in this list (if hits > 0). Now get the name of the closest picked object (depth information is smallest, see OpenGL's red book

  if (hits > 0)
    {
      unsigned int k;
      GLuint names;
      GLuint* ptr;
      GLuint minZ;
      GLuint closest_name_id = 0;
      
      ptr = selectBuf;
      minZ = 0xffffffff;
      for (k = 0; k < hits; k++) 
	{ 
	  names = *ptr;
	  ptr++;
	  if (*ptr < minZ) 
	    {
	      minZ = *ptr;
	      closest_name_id = *(ptr + 2);
	    }
	  ptr += 2 + names;
	}

      //the name of the closest name is now known and stored in closest_name_id
      //so find and return it
      
      list<WP_StaticObject*>::const_iterator i = static_objects.begin();
      while (i != static_objects.end())
	{	
	  if ((*i)->name_id == closest_name_id)
	    {
	      return (*i);
	    }
	  i++;
	}
  
      list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
      while (j != dynamic_objects.end())
	{	
	  if ((*j)->name_id == closest_name_id)
	    {
	      return (*j);
	    }
	  j++;
	}
    }

  return (WP_Object*)0;
}

//**************************************************************************************

WP_DynamicObject::WP_DynamicObject(const WP_Matrix3D& _matrix, const string& name, const WP_Vector3D& _velocity):
  WP_Object(_matrix, name), velocity(_velocity)
{
  //orientate object according to velocity vector, object should be orientated according to 0 degrees on a compass 

  heading = computeHeading();
  WP_Matrix3D yrot(Y_ROTATION_MATRIX, -heading); //negative heading because the rotation in WP_Matrix3D is Counter Clockwise (CCW) and it should be CW
  matrix *= yrot;

  //set speed, speed is length of the velocity vector
  speed = sqrtf((velocity.data[0] * velocity.data[0]) + (velocity.data[1] * velocity.data[1]) + (velocity.data[2] * velocity.data[2]));

  //now calculate the rotation on the x-axis, see page 831 of HILL

  pitch = computePitch();
  if (pitch != 0.0f)
    {
      WP_Matrix3D xrot(X_ROTATION_MATRIX, pitch);
   
      matrix *= xrot;
      
      dir *= xrot;
      up *= xrot;
    }

  dir *= yrot;
  up *= yrot;

  dir.normalize();
  up.normalize();
}

WP_DynamicObject::WP_DynamicObject(const WP_Matrix3D& _matrix, const string& name, scalar _heading, scalar _speed, 
				   scalar _pitch):WP_Object(_matrix, name), speed(_speed)
{
  heading = _heading;
  pitch = _pitch;

  velocity = computeVelocityVector();
  WP_Matrix3D yrot(Y_ROTATION_MATRIX, -heading); //negative heading because the rotation in WP_Matrix3D is Counter Clockwise (CCW) and it should be CW
  matrix *= yrot;
  
  //now calculate the rotation on the x-axis, see page 831 of HILL

  if (pitch != 0.0f)
    {
      WP_Matrix3D xrot(X_ROTATION_MATRIX, pitch);
      matrix *= xrot;
    }

  dir.normalize();
  up.normalize();
}

scalar WP_DynamicObject::computeHeading() const
{
  //first calculate rotation on y-axis, see page 831 of Hill, Computer Graphics using OpenGL
  
  if (velocity.data[0] == 0.0f && velocity.data[2] == 0.0f)
    {
      return 0.0f;
    }
  else
    {
      //first the outcome of the atan2 function was compared to the actual orientation on the unitcircle
      //the following table was made
      //
      //outcome          unitcircle
      //
      // 1/2PI               -1/2PI
      // -1/2PI               1/2PI
      // 0                    0
      // PI                   PI
      // 1/4PI                -1/4PI
      // etc                  etc
      
      //this table shows that when the z (velocity.data[2]) != 0.0, the outcome has to be inverted to match the correct
      // unitcircle position. When the z == 0.0 the position on the unit circle is correct.

      scalar course = atan2(velocity.data[2] , velocity.data[0]);

      if (velocity.data[2] != 0.0f)
      	{
	  course = -course;
	}

      //now that the course matches the correct position on the unitcircle it has got to be corrected to match a compass orientation
      //compared to the unitcircle a compass starts at 1/2PI with 0 degrees (north), 0 is 90 degrees (east), -1/2PI = 180 degrees(south),
      //PI is 270 degrees(west). The compass its orientation is clockwise!.
      //The above mentioned values show that when the course is corrected by course = HALF_PI - course, the values are correctly matched on
      //the compass, so the formula is 1/2PI - course.

      course = HALF_PI - course;

      return math->fRadToDegree(course);
      
    }
}

//this function computes the velocity vector according to the known heading, speed and pitch
WP_Vector3D WP_DynamicObject::computeVelocityVector() 
{
  //check for zero speed
  if (speed == 0.0f)
    {
      return WP_Vector3D(0.0f, 0.0f, 0.0f);
    }

  //first convert compass heading to unitcircle heading
  scalar course = math->fDegreeToRad(heading);
  course = HALF_PI - course;

  //determine x and z component of resulting vector

  WP_Vector3D vec(cos(course), 0.0f, -sin(course));

  //rotate vector according to its pitch
  //first rotate it pointing to 0 degrees (north);

    if (pitch != 0.0f)
    {
      scalar difference = 360.0f - heading; 
      if (difference !=  0)
	{
	  WP_Matrix3D yrot(Y_ROTATION_MATRIX, -difference); 
	  vec *= yrot;
	}
  
      //now rotate around x-axis pitch degrees
      WP_Matrix3D xrot(X_ROTATION_MATRIX, pitch);

      vec *= xrot;

      dir *= xrot;
      up *= xrot;

      //rotate vector back to heading
 
      if (difference !=  0)
	{
	  WP_Matrix3D yrot(Y_ROTATION_MATRIX, difference); 
	  WP_Matrix3D yrot2(Y_ROTATION_MATRIX, difference - 360.0f);
	  vec *= yrot;

	  dir *= yrot2;
	  up *= yrot2;
	}
    }

    vec.data[0] *= speed;
    vec.data[1] *= speed;
    vec.data[2] *= speed;

    return vec;
}

scalar WP_DynamicObject::computePitch() const
{
  //now calculate the rotation on the x-axis, see page 831 of HILL

  if (speed != 0.0f)
    {
      return math->fRadToDegree(asin(velocity.data[1] / speed));
    }
  return 0;
}

void WP_DynamicObject::setVelocityVector(const WP_Vector3D& vector)
{
  velocity = vector;
  scalar new_heading = computeHeading();
  setNewHeading(new_heading);
  speed = sqrtf((velocity.data[0] * velocity.data[0]) + (velocity.data[1] * velocity.data[1]) + (velocity.data[2] * velocity.data[2]));
  scalar new_pitch = computePitch();
  setNewPitch(new_pitch);
}

//adds degrees to the current heading
void WP_DynamicObject::changeHeading(scalar delta_degrees)
{
  setNewHeading(heading + delta_degrees);
}

void WP_DynamicObject::changePitch(scalar delta_degrees)
{
  setNewPitch(pitch + delta_degrees);
}

void WP_DynamicObject::changeRoll(scalar delta_degrees)
{
  setNewRoll(roll + delta_degrees);
}

//orientates object according to the new heading
//rotation is CW
void WP_DynamicObject::setNewHeading(scalar new_heading)
{
  if (heading != new_heading)
    {
      if (new_heading >= 360.0f)
	{
	  new_heading -= 360.0f;
	}
      else if (new_heading < 0.0f)
	{
	  new_heading += 360.0f;
	}

      scalar difference = heading - new_heading;
      if (difference !=  0.0f)
	{
	  WP_Matrix3D yrot(Y_ROTATION_MATRIX, difference); 
	  matrix *= yrot;
	  heading = new_heading;

	  //adjust dir and up

	  dir *= yrot;
	  up *= yrot;

	  //change velocity vector too

	  velocity *= yrot;
	}
    }
}

//orientates object according to the new pitch
//90 degrees is up, 270 degrees is down
void WP_DynamicObject::setNewPitch(scalar new_pitch)
{
  if (pitch != new_pitch)
    {
      if (new_pitch >= 360.0f)
	{
	  new_pitch -= 360.0f;
	}
      else if (new_pitch < 0.0f)
	{
	  new_pitch += 360.0f;
	}

      scalar difference = new_pitch - pitch;
      if (difference != 0.0f)
	{
	  WP_Matrix3D xrot(X_ROTATION_MATRIX, difference);
	  matrix *= xrot;
	  pitch = new_pitch;

	  //change velocity vector too
	  WP_Matrix3D xrot2(X_ROTATION_MATRIX, -difference);
	  velocity *= xrot2;

	  //adjust dir and up

	  dir *= xrot2; //FIXME IS THIS CORRECT?
	  up *= xrot2;
	}
    }
}

//orientates object according to the new pitch
//roll is in CW order
void WP_DynamicObject::setNewRoll(scalar new_roll)
{
  if (roll != new_roll)
    {
      if (new_roll >= 360.0f)
	{
	  new_roll -= 360.0f;
	}
      else if (new_roll < 0.0f)
	{
	  new_roll += 360.0f;
	}

      scalar difference = new_roll - roll;
      if (difference != 0.0f)
	{
	  WP_Matrix3D zrot(Z_ROTATION_MATRIX, difference);
	  matrix *= zrot;
	  roll = new_roll;

	  //adjust up

	  up *= zrot;
	}
    }
}

void WP_DynamicObject::changeSpeed(scalar delta_speed)
{
  setNewSpeed(speed + delta_speed);
}

void WP_DynamicObject::setNewSpeed(scalar new_speed)
{
  if (new_speed != speed)
    {
      scalar multiply = 0.0f;
 
      if (speed == 0.0f)
	{
	  //compute new velocity vector, because object starts moving again
	  speed = new_speed;
	  velocity = computeVelocityVector();
	}
      else
	{
	  multiply = new_speed / speed;
	  speed = new_speed;
	  velocity *= multiply;
	}
   }
}

void WP_DynamicObject::move()
{
  matrix.data[12] += velocity.data[0];
  matrix.data[13] += velocity.data[1];
  matrix.data[14] += velocity.data[2];
}

void WP_DynamicObject::print() const
{
  WP_Object::print();
  cout << "Speed: " << speed << endl;
  cout << "Velocity vector: x:" << velocity.data[0] << " y:" << velocity.data[1] << " z:" << velocity.data[2] << endl;
}

//OPCODE CALLBACK
void 
WP_ObjectManager::ColCallback(udword triangleindex, VertexPointers &triangle, uqword user_data)
{
  WP_Object* object = reinterpret_cast<WP_Object*>(user_data);
  WP_Model *model = object->model;
  unsigned int* indices = model->triangles + triangleindex * 3;

  triangle.Vertex[0] = reinterpret_cast<Point*>(&model->getVertex(object, indices[0])->point);
  triangle.Vertex[1] = reinterpret_cast<Point*>(&model->getVertex(object, indices[1])->point);
  triangle.Vertex[2] = reinterpret_cast<Point*>(&model->getVertex(object, indices[2])->point);
}

void 
WP_ObjectManager::addLight(const WP_Point3D& position, const WP_Color& ambient, const WP_Color& diffuse, const WP_Color& specular, const WP_Color& emissive, bool remote)
{
  if (number_lights < 8)  // maximal 8 light sources are permitted in OpenGL
    {
      WP_Light* l = new WP_Light(number_lights, position, ambient, diffuse, specular, emissive, remote);
      lights.push_back(l);

      glLightfv(GL_LIGHT0 + number_lights, GL_AMBIENT, l->ambient_color.components);  
      glLightfv(GL_LIGHT0 + number_lights, GL_DIFFUSE, l->diffuse_color.components);  
      glLightfv(GL_LIGHT0 + number_lights, GL_SPECULAR,l->specular_color.components);  
      glLightfv(GL_LIGHT0 + number_lights, GL_EMISSION,l->emissive_color.components);  
      WP_GLState::getInstance()->enableLighti(number_lights);
      if (number_lights == 0)
	{
	  WP_GLState::getInstance()->enableLighting();
	  glEnable(GL_COLOR_MATERIAL);
	  glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
	}
      number_lights++;
    }
}  

bool 
WP_ObjectManager::notInterpolatedFrame()
{
	static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
	return ani->getT(*dynamic_objects.begin()) == 0.0f;
}

void 
WP_ObjectManager::updateAll()
{
  static WP_AnimationManager *ani = WP_AnimationManager::getInstance();
	
  //move dynamic objects
  list<WP_DynamicObject*>::iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      //update animation
      ani->updateAnimation(*j);
      //move
      (*j)->move();
      j++;
    }

  cam->followObject();
  
  //Dynamic OBJECT IN FRUSTUM??
  j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      PC.SetCallback(&ColCallback, uqword(*j));
    
      if (PC.Collide((*j)->planesCache, cam->getFrustum(), 6, (*j)->model->getCollisionModel(*j), 
		     reinterpret_cast<Matrix4x4*>(&(*j)->matrix)))
	{
		(*j)->inFrustum = PC.GetContactStatus();
	}
      else
	cout << "COLLIDING ERROR" << endl;
      j++;
    }
      
  list<WP_StaticObject*>::iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
	//update animation
      ani->updateAnimation(*i);

      //OBJECT IN FRUSTUM??
      
      PC.SetCallback(&ColCallback, uqword(*i));
    
      if (PC.Collide((*i)->planesCache, cam->getFrustum(), 6, (*i)->model->getCollisionModel(*i), 
		     reinterpret_cast<Matrix4x4*>(&(*i)->matrix)))
	{
		(*i)->inFrustum = PC.GetContactStatus();
	}
      else
	cout << "COLLIDING ERROR" << endl;
      i++;
    }

  if (notInterpolatedFrame())	//FIXME because no collision models are created for interpolated frames only check collisions when animation is on start of frame
  	checkCollisions();
}

void
WP_ObjectManager::createCollisionPairs()
{
  list<WP_DynamicObject*>::const_iterator i = dynamic_objects.begin();
  
  while (i != dynamic_objects.end())
    {
      // create dynamic - static pairs

      list<WP_StaticObject*>::const_iterator j = static_objects.begin();
      while (j != static_objects.end())
	{
	  WP_ObjectManager::WP_CollisionPair *pair = new WP_ObjectManager::WP_CollisionPair((*i), (*j));
	  collision_pairs.push_back(pair);
	  ++j;
	}

      // create dynamic - dynamic pairs

      list<WP_DynamicObject*>::const_iterator k = i;
      ++k;

      while (k != dynamic_objects.end())
	{
	  WP_ObjectManager::WP_CollisionPair *pair = new WP_ObjectManager::WP_CollisionPair((*i), (*k));
	  collision_pairs.push_back(pair);
	  ++k;
	}
      ++i;
    }
}

WP_ObjectManager::WP_CollisionPair::WP_CollisionPair(WP_Object *obj1, WP_Object *obj2)
  :object1(obj1), object2(obj2)
{
  cache.Model0 = object1->model->getCollisionModel(object1);
  cache.Model1 = object2->model->getCollisionModel(object2); 
}

WP_Light* WP_ObjectManager::getLight(int index)
	{
		if (index < lights.size())
		{
  			list<WP_Light*>::const_iterator i = lights.begin();
			int c = 0;
			while (i != lights.end())
			{
				if (c == index)
					return *i;
				i++; 
			}
		}
		return NULL;
	}

}

