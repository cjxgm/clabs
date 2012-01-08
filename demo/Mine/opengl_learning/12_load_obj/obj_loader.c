
#include "obj_loader.h"
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>




void skip_blank(FILE * fp)
{
	int ch = 0;

	while (1) {
		int end = 1;
		while ((ch = getc(fp)) == ' ' ||
				ch == '\t' || ch == '\r' || ch == '\n') { end = 0; }
		if (ch == '#') {
			while ((ch = getc(fp)) != '\n' &&
					ch != '\r' && ch != EOF) {}
			end = 0;
		}
		ungetc(ch, fp);
		if (end) break;
	}
}


//  0 --> failed
//  1 --> succeeded
// -1 --> eof
int read_int(FILE * fp, int * v)
{
	skip_blank(fp);
	return fscanf(fp, "%d", v);
}

int read_float(FILE * fp, float * v)
{
	skip_blank(fp);
	return fscanf(fp, "%g", v);
}

int read_letter(FILE * fp, char * v)
{
	skip_blank(fp);
	return fscanf(fp, "%c", v);
}



Obj * load_obj(FILE * fp)
{
	float pos[3];
	char  vc;

	int vid = 0;
	Link vs;
	link_init(&vs);

	Vert ** vvs = NULL;

	Obj * o = malloc(sizeof(Obj));
	link_init(&o->face);

	while (1) {
		int r = read_letter(fp, &vc);
		if (r == -1) break;
		if (r == 0) {
			free(o);
			fprintf(stderr, "Letter expected.\n");
			return NULL;
		}
		switch (vc) {
			case 'v':
				if (read_float(fp, &pos[0]) &&
						read_float(fp, &pos[1]) &&
						read_float(fp, &pos[2])) {
					Vert * vt = malloc(sizeof(Vert));
					vt->pos[0] = pos[0];
					vt->pos[1] = pos[1];
					vt->pos[2] = pos[2];
					LinkVert * lv = malloc(sizeof(LinkVert));
					lv->v = vt;
					link_addtail(&vs, lv);
					vid++;
				}
				else {
					free(o);
					fprintf(stderr, "3 floats expected. Got %g %g %g\n",
							pos[0], pos[1], pos[2]);
					return NULL;
				}
				break;
			case 'f':
				if (vvs == NULL) {
					vvs = malloc(sizeof(Vert *) * vid);
					int i = 0;
					LinkVert * q = (LinkVert *)&vs;
					while (q->next != (LinkVert*)&vs) {
						LinkVert * p = q->next;
						vvs[i++] = p->v;
						link_delete(p);
					}
				}

				{
					LinkFace * lf = malloc(sizeof(LinkFace));
					link_init(&lf->vert);
					link_addtail(&o->face, lf);

					int i;
					while (read_int(fp, &i) == 1) {
						LinkVert * lv = malloc(sizeof(LinkVert));
						lv->v = vvs[(i-1)%vid];
						link_addtail(&lf->vert, lv);
					}
				}
				break;
			default:
				free(o);
				fprintf(stderr, "Letter 'v' or 'f' expected.\n");
				return NULL;
		}
	}
	free(vvs);
	printf("done\n");
	{
		int i = 0;
		TRAVERSE(LinkFace, f, &o->face) {
			printf("face%d:\n", i++);
			TRAVERSE(LinkVert, v, &f->vert) {
				printf("\t%g %g %g\n",
						v->v->pos[0], v->v->pos[1], v->v->pos[2]);
			}
		}
	}
	return o;
}

void get_normal(float n[], float v1[], float v2[], float v3[])
{
	float a[3] = {v2[0]-v1[0], v2[1]-v1[1], v2[2]-v1[2]};
	float b[3] = {v3[0]-v1[0], v3[1]-v1[1], v3[2]-v1[2]};
	n[0] = a[1]*b[2] - a[2]*b[1];
	n[1] = a[2]*b[0] - a[0]*b[2];
	n[2] = a[0]*b[1] - a[1]*b[0];
	float d = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] /= d;
	n[1] /= d;
	n[2] /= d;
}

void draw_obj(Obj * o)
{
	glColor3f(0.5, 0.5, 0.5);
	TRAVERSE(LinkFace, f, &o->face) {
		glBegin(GL_POLYGON);
		{
			float n[3];
			LinkVert * v = (LinkVert *)&f->vert;
			v = v->next;
			get_normal(n, v->v->pos, v->next->v->pos,
							v->next->next->v->pos);
			glNormal3fv(n);
		}
		TRAVERSE(LinkVert, v, &f->vert) {
			glVertex3fv(v->v->pos);
		}
		glEnd();
	}
}

