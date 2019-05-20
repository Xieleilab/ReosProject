#ifndef TST_001_HDMESHGENERATOR_H
#define TST_001_HDMESHGENERATOR_H

#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../../Mesher/HdMesh/hdmeshgenerator.h"
#include "../../Mesher/HdTin/hdtin.h"


using namespace testing;

class MeshGeneratorTesting:public Test{
public:

    HdMeshGeneratorTriangleFile meshGeneratorTriangleFile;

    void populateWithVertex()
    {
        inputMesh.addVertex(5,15);
        inputMesh.addVertex(10,15);
        inputMesh.addVertex(5,10);
        inputMesh.addVertex(10,10);
        inputMesh.addVertex(10,5);
    }

    void populateSegment()
    {
        populateWithVertex();
        //segmentList.push_back(Segment(0,4));
    }

    void triangulateWithVertex()
    {
       populateWithVertex();
       meshGeneratorTriangleFile.triangulateMesh(inputMesh,outputMesh);
    }

    void triangulateWithSegment(std::vector<VertexBasic> &oVert,std::vector<Face> &oFaces)
    {
        populateSegment();
        ///TODO
       // meshGeneratorTriangleFile.triangulateTIN(verticesList,segmentList,oFaces);
    }

    HdMeshBasic inputMesh;
    std::vector<Segment> segmentList;

    HdMeshBasic outputMesh;
    std::vector<Segment> outputSegments;
};

TEST_F(MeshGeneratorTesting, triangulateFailNotEnoughtPoint)
{
    inputMesh.addVertex(0,1);

    ASSERT_FALSE(meshGeneratorTriangleFile.triangulateMesh(inputMesh,outputMesh));
    ASSERT_THAT(meshGeneratorTriangleFile.getError(),Eq("NOT_ENOUGHT_DATA"));
}


TEST_F(MeshGeneratorTesting, triangulateMeshSuccessWithVertex)
{
    populateWithVertex();
    ASSERT_TRUE(meshGeneratorTriangleFile.triangulateMesh(inputMesh,outputMesh));
}

TEST_F(MeshGeneratorTesting, triangulateMeshSuccessWithSegment)
{
    populateSegment();
    ASSERT_TRUE(meshGeneratorTriangleFile.triangulateTIN(inputMesh,segmentList));
}

TEST_F(MeshGeneratorTesting, incrementSimpleFileName)
{
    std::string filename="abcd";
    ASSERT_THAT(meshGeneratorTriangleFile.incFileName(filename),Eq("abcd.1"));
}

TEST_F(MeshGeneratorTesting, incrementIncrementedFileName)
{
    std::string filename="abcd.1.node";
    ASSERT_THAT(meshGeneratorTriangleFile.incFileName(filename),Eq("abcd.2.node"));
}

TEST_F(MeshGeneratorTesting, FaceCount)
{
    triangulateWithVertex();

    ASSERT_THAT(outputMesh.facesCount(),Eq(3));
}

TEST_F(MeshGeneratorTesting, VertexCount)
{
    triangulateWithVertex();

    ASSERT_THAT(outputMesh.verticesCount(),Eq(5));
}



class MeshGeneratorCGALTesting:public Test{
public:
    HdTin cgalMESH;


};

TEST_F(MeshGeneratorCGALTesting, meshCreation)
{
    ASSERT_THAT(cgalMESH.verticesCount(),Eq(0));
}

TEST_F(MeshGeneratorCGALTesting, addOneVertex)
{
    cgalMESH.addVertex(5,5);

    ASSERT_THAT(cgalMESH.verticesCount(),Eq(1));
}

TEST_F(MeshGeneratorCGALTesting, addVerticesToHaveAFace)
{
    cgalMESH.addVertex(5,5);
    cgalMESH.addVertex(10,5);
    cgalMESH.addVertex(5,10);

    ASSERT_THAT(cgalMESH.verticesCount(),Eq(3));
    ASSERT_THAT(cgalMESH.facesCount(),Eq(1));
}

TEST_F(MeshGeneratorCGALTesting, addVerticesToHaveTwoFace)
{
    cgalMESH.addVertex(5,5);
    cgalMESH.addVertex(6,5);
    cgalMESH.addVertex(7,10);
    cgalMESH.addVertex(10,10);

    ASSERT_THAT(cgalMESH.verticesCount(),Eq(4));
    ASSERT_THAT(cgalMESH.facesCount(),Eq(2));
}


TEST_F(MeshGeneratorCGALTesting, addDupplicatesVertices)
{
    cgalMESH.addVertex(5,5);
    cgalMESH.addVertex(5,5);
    cgalMESH.addVertex(7,10);
    cgalMESH.addVertex(10,10);

    ASSERT_THAT(cgalMESH.verticesCount(),Eq(3));
    ASSERT_THAT(cgalMESH.facesCount(),Eq(1));
}

TEST_F(MeshGeneratorCGALTesting, locateVertex)
{
    VertexPointer vert1=cgalMESH.addVertex(5,5);

    cgalMESH.addVertex(7,10);
    //cgalMESH.addVertex(10,10);

    VertexPointer vert2=cgalMESH.vertex(4.999,5.001,0.01);

    ASSERT_TRUE(vert2==vert1);

}


#endif // TST_001_HDMESHGENERATOR_H
