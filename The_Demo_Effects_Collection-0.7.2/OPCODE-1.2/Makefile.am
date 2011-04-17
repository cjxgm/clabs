#file OPC_TreeCollider.cpp and OPC_RayCollider.cpp generate run-time errors when compiled with optimisation -O2 when using gcc >= 3.0. 
#Therefore this check is made, so when gcc >= 3.0,file OPC_TreeCollider.cpp is compiled with -O1

if GCCVERSION3
OPC_TreeCollider.o:
	source='OPC_TreeCollider.cpp' object='OPC_TreeCollider.o' libtool=no \
	depfile='$(DEPDIR)/OPC_TreeCollider.Po' tmpdepfile='$(DEPDIR)/OPC_TreeCollider.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -O1 -c -o OPC_TreeCollider.o `test -f 'OPC_TreeCollider.cpp' || echo '$(srcdir)/'`OPC_TreeCollider.cpp
OPC_RayCollider.o:
	source='OPC_RayCollider.cpp' object='OPC_RayCollider.o' libtool=no \
	depfile='$(DEPDIR)/OPC_RayCollider.Po' tmpdepfile='$(DEPDIR)/OPC_RayCollider.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -O1 -c -o OPC_RayCollider.o `test -f 'OPC_RayCollider.cpp' || echo '$(srcdir)/'`OPC_RayCollider.cpp
endif

if OPENGL
lib_LIBRARIES = libOPCODE.a
else
lib_LIBRARIES = 
endif

libOPCODEincludedir = $(includedir)/OPCODE-1.2
libOPCODEinclude_HEADERS = *.h

libOPCODE_a_SOURCES = Opcode.cpp OPC_BVTCache.h     OPC_MemoryMacros.h     OPC_OptimizedTree.h      OPC_Ray.cpp  OPC_Triangle.h \
	OPC_Collider.cpp   OPC_Model.cpp          OPC_Plane.cpp            OPC_Ray.h                OPC_TriBoxOverlap.h \
        OPC_Collider.h     OPC_Model.h            OPC_Plane.h              OPC_RayTriOverlap.h      OPC_TriTriOverlap.h \
	OPC_Common.cpp     OPC_OBBCollider.cpp    OPC_PlanesAABBOverlap.h  OPC_Settings.h           OPC_Types.h \
	OPC_Common.h       OPC_OBBCollider.h      OPC_PlanesCollider.cpp   OPC_SphereAABBOverlap.h  OPC_VolumeCollider.cpp \
	OPC_AABBCollider.cpp  OPC_Container.cpp  OPC_OBB.cpp            OPC_PlanesCollider.h     OPC_SphereCollider.cpp   \
	OPC_VolumeCollider.h OPC_AABBCollider.h    OPC_Container.h    OPC_OBB.h	OPC_PlanesTriOverlap.h   OPC_SphereCollider.h \
	OPC_AABB.cpp          OPC_FPU.h          OPC_Point.cpp            OPC_SphereTriOverlap.h   \
	OPC_AABB.h            OPC_HPoint.h       OPC_Point.h              OPC_TreeBuilders.cpp     \
	OPC_AABBTree.cpp      OPC_Matrix3x3.cpp  OPC_Preprocessor.h       OPC_TreeBuilders.h	\
	OPC_AABBTree.h        OPC_Matrix3x3.h    Opcode.h               OPC_RayAABBOverlap.h     OPC_TreeCollider.cpp	\
	OPC_BoundingSphere.h  OPC_Matrix4x4.cpp  OPC_RayCollider.cpp      OPC_TreeCollider.h	\
	OPC_BoxBoxOverlap.h   OPC_Matrix4x4.h    OPC_OptimizedTree.cpp  OPC_RayCollider.h        OPC_Triangle.cpp	\
	Stdafx.h	Stdafx.cpp
