if OPENGL
lib_LIBRARIES = libWPCG.a
else
lib_LIBRARIES =
endif

libWPCGincludedir = $(includedir)/WPCG
libWPCGinclude_HEADERS = *.h
libWPCG_a_SOURCES=WP_2D.C WP_Camera.C WP_Endian.C WP_GLState.C WP_Image.C WP_Init.C WP_Line2D.C WP_Material.C WP_Math.C WP_Matrix2D.C \
WP_Matrix3D.C WP_Model.C WP_ObjectManager.C WP_Point2D.C WP_Point3D.C WP_Ray2D.C WP_Ray3D.C WP_SkyBox.C WP_Terrain.C \
WP_TextureManager.C WP_Triangle.C WP_Vector2D.C WP_Vector3D.C WP_AnimationManager.C
EXTRA_DIST=*.h
INCLUDES= -I../OPCODE-1.2

