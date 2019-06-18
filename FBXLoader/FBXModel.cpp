//
//  FBXImporter.cpp
//  FBXLoader
//
//  Created by Jeehoon Hyun on 11/06/2019.
//  Copyright © 2019 Jeehoon Hyun. All rights reserved.
//

#include "FBXModel.h"


void PrintMatrix(const FbxAMatrix& pMatrix)
{
    
    //print the TRS
    FBXSDK_printf(" T : %f %f %f %f\n",
                  pMatrix.GetT()[0],
                  pMatrix.GetT()[1],
                  pMatrix.GetT()[2],
                  pMatrix.GetT()[3] );
    
    FBXSDK_printf(" R : %f %f %f %f\n",
                  pMatrix.GetR()[0],
                  pMatrix.GetR()[1],
                  pMatrix.GetR()[2],
                  pMatrix.GetR()[3] );
    
    FBXSDK_printf(" S : %f %f %f %f\n",
                  pMatrix.GetS()[0],
                  pMatrix.GetS()[1],
                  pMatrix.GetS()[2],
                  pMatrix.GetS()[3] );
}

void FBXModel::ReadPosition(FbxMesh* mesh, int ctrlPointIndex, glm::vec3& position)
{
    FbxVector4* controlPoints = mesh->GetControlPoints();
    unsigned int ctrlPointCount = mesh->GetControlPointsCount();
    for(unsigned int i = 0; i < ctrlPointCount; i++)
    {
        position.x = controlPoints[ctrlPointIndex].mData[0];
        position.y = controlPoints[ctrlPointIndex].mData[1];
        position.z = controlPoints[ctrlPointIndex].mData[2];
    }
}

void FBXModel::ReadUV(FbxMesh* mesh, int inCtrlPointIndex, int textureUVIndex, glm::vec2& UV){
    if(mesh->GetElementNormalCount() < 1)
    {
        throw std::exception();
    }
    
    FbxGeometryElementUV * vertexUV = mesh->GetElementUV(0);
    
    switch(vertexUV->GetMappingMode())
    {
        case FbxGeometryElement::eByControlPoint:
            switch(vertexUV->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
            {
                std::cout << "eByControlPoint" << std::endl;
                UV.x = vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0];
                UV.y = vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1];
            }
                break;
                
            case FbxGeometryElement::eIndexToDirect:
            {
                std::cout << "eByControlPoint" << std::endl;
                int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
                UV.x = vertexUV->GetDirectArray().GetAt(index).mData[0];
                UV.y = vertexUV->GetDirectArray().GetAt(index).mData[1];
            }
                break;
                
            default:{
                //std::cout << "ERROR1" << std::endl;
                throw std::exception();
            }
        }
            break;
            
        case FbxGeometryElement::eByPolygonVertex:
            switch(vertexUV->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
            {
                std::cout << "eByVertex" << std::endl;
                UV.x = vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[0];
                UV.y = vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[1];
            }
                break;
                
            case FbxGeometryElement::eIndexToDirect:
            {
                //pirate example is here.
                //std::cout << "eByVertex" << std::endl;
                //int index = vertexUV->GetIndexArray().GetAt(textureUVIndex);
                //Hmm I don't get it but it worked lools
                UV.x = vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[0];
                UV.y = vertexUV->GetDirectArray().GetAt(textureUVIndex).mData[1];
            }
                break;
                
            default:{
                //std::cout << "ERROR2" << std::endl;
                throw std::exception();
            }
        }
    }
    //std::cout << "UV is "<< UV.x << ", " << UV.y << std::endl;
}

void FBXModel::ReadNormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, glm::vec3& outNormal)
{
    if(mesh->GetElementNormalCount() < 1)
    {
        throw std::exception();
    }
    
    FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
    
    switch(vertexNormal->GetMappingMode())
    {
        case FbxGeometryElement::eByControlPoint:
            switch(vertexNormal->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
            {
                //std::cout << "eByControlPoint" << std::endl;
                outNormal.x = vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0];
                outNormal.y = vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1];
                outNormal.z = vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2];
            }
                break;
                
            case FbxGeometryElement::eIndexToDirect:
            {
                //std::cout << "eByControlPoint" << std::endl;
                int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
                outNormal.x = vertexNormal->GetDirectArray().GetAt(index).mData[0];
                outNormal.y = vertexNormal->GetDirectArray().GetAt(index).mData[1];
                outNormal.z = vertexNormal->GetDirectArray().GetAt(index).mData[2];
            }
                break;
                
            default:{
                //std::cout << "ERROR1" << std::endl;
                throw std::exception();
            }
        }
            break;
            
        case FbxGeometryElement::eByPolygonVertex:
            switch(vertexNormal->GetReferenceMode())
        {
            case FbxGeometryElement::eDirect:
            {
                //std::cout << "eByVertex" << std::endl;
                outNormal.x = vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0];
                outNormal.y = vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1];
                outNormal.z = vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2];
            }
                break;
                
            case FbxGeometryElement::eIndexToDirect:
            {
                //std::cout << "eByVertex" << std::endl;
                int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
                outNormal.x = vertexNormal->GetDirectArray().GetAt(index).mData[0];
                outNormal.y = vertexNormal->GetDirectArray().GetAt(index).mData[1];
                outNormal.z = vertexNormal->GetDirectArray().GetAt(index).mData[2];
            }
                break;
                
            default:{
                //std::cout << "ERROR2" << std::endl;
                throw std::exception();
            }
        }
    }
}

void FBXModel::ProcessMesh(FbxNode* node, std::vector<Vertex> * vertices){
    //First check if the child node is a mesh node.
    if(node->GetNodeAttribute() == NULL){
        std::cout << "Attribute is NULL" << std::endl;
        return;
    }
    FbxNodeAttribute::EType AttributeType = node->GetNodeAttribute()->GetAttributeType();
    if(AttributeType != FbxNodeAttribute::eMesh){
        if(AttributeType == FbxNodeAttribute::eSkeleton)
            std::cout << "Attribute is skeleton" << std::endl;
        return;
    }
    
    FbxMesh* currMesh = (FbxMesh *) node->GetNodeAttribute();
    int mTriangleCount = currMesh->GetPolygonCount();
    std::cout << "Number of polygons are " << mTriangleCount << std::endl;
    int vertexCounter = 0;
    
    for (unsigned int i = 0; i < mTriangleCount; ++i)
    {
        glm::vec3 position[3];
        glm::vec3 normal[3];
        glm::vec2 UV[3];
        for (unsigned int j = 0; j < 3; ++j)
        {
            int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
            ReadPosition(currMesh, ctrlPointIndex, position[j]);
            ReadNormal(currMesh, ctrlPointIndex, vertexCounter, normal[j]);
            //std::cout << "Vertex Counter is "<< vertexCounter << std::endl;
            ReadUV (currMesh, ctrlPointIndex, currMesh->GetTextureUVIndex(i, j), UV[j]);
            Vertex vertex;
            vertex.position = position[j];
            vertex.normal = normal[j];
            vertex.UV = UV[j];
            vertices->push_back(vertex);
            vertexCounter++;
        }
    }
    std::cout << "Number of 'vertices': " << vertices->size() << std::endl;
}

void FBXModel::ProcessSkeletonHierarchy(FbxNode* rootNode){
    for(int childIndex=0;childIndex < rootNode->GetChildCount();childIndex++){
        FbxNode* currNode = rootNode->GetChild(childIndex);
        //Root of the SKELETON!! (not the Fbx root node) has the index 0. Index increases in DFS order.
        ProcessSkeletonHierarchyRecursively(currNode, 0, -1);
    }
    
    //std::cout << "number of joints are "<< rig.Joints.size() << std::endl;
}

void FBXModel::ProcessSkeletonHierarchyRecursively(FbxNode * node, int myIndex, int parentIndex){
    if(node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType()== FbxNodeAttribute::eSkeleton){
        Joint currJoint;
        currJoint.mParentIndex = parentIndex;
        currJoint.mName = node->GetName();
        //std::cout << currJoint.mName << std::endl; //For debugging
        rig.Joints.push_back(currJoint);
    }
    else{
        return;
    }
    //Process Skeleton in DFS way.
    for(int i=0;i<node->GetChildCount();i++){
        ProcessSkeletonHierarchyRecursively(node->GetChild(i), rig.Joints.size(), myIndex);
    }
}

int FBXModel::findJointIndexUsingName(const char * jointName){
    for(int i=0;i<rig.Joints.size();i++){
        if(strcmp(rig.Joints.at(i).mName, jointName)==0){
            return i;
        }
    }
    //Something is wrong if they haven't found the index til here.
    return -1;
}

void FBXModel::ProcessJointsAndAnimation(FbxNode* node){
    if(node->GetNodeAttribute() == NULL){
        std::cout << "Attribute is NULL" << std::endl;
        return;
    }
    FbxNodeAttribute::EType AttributeType = node->GetNodeAttribute()->GetAttributeType();
    if(AttributeType != FbxNodeAttribute::eMesh){
        if(AttributeType == FbxNodeAttribute::eSkeleton)
            std::cout << "Attribute is skeleton" << std::endl;
        return;
    }
    FbxMesh * currMesh = node->GetMesh();
    int numOfDeformers = currMesh->GetDeformerCount();
    
    std::cout << node->GetName() << std::endl;
    
    for(int deformerIndex = 0;deformerIndex < numOfDeformers;deformerIndex++){
        FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
        if(!currSkin){
            continue;
        }
        int numOfClusters = currSkin->GetClusterCount();
        //For each joint
        for(int clusterIndex=0;clusterIndex < numOfClusters;clusterIndex++){
            FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
            const char * currJointName = currCluster->GetLink()->GetName();
            int currJointIndex = findJointIndexUsingName(currJointName);
            std::cout << currJointName << std::endl;
            std::cout << "currJointIndex is " << currJointIndex << std::endl;
            FbxAMatrix transformMatrix;
            FbxAMatrix transformLinkMatrix;
            FbxAMatrix globalBindPoseInverseMatrix;
            currCluster->GetTransformMatrix(transformMatrix);
            currCluster->GetTransformLinkMatrix(transformLinkMatrix);
            globalBindPoseInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix;
            
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    rig.Joints[currJointIndex].globalBindPoseInverse[i][j] = globalBindPoseInverseMatrix.Get(i, j);
                }
            }
            
            //rig.Joints[currJointIndex].globalBindPoseInverse = glm::mat4(1.0);
            
            rig.Joints[currJointIndex].node = currCluster->GetLink();
            //controlpointindices to controlpointweights is always a one-to-one relation
            rig.Joints[currJointIndex].controlPointIndices = currCluster->GetControlPointIndices();
            rig.Joints[currJointIndex].weights = currCluster->GetControlPointWeights();
            //std::cout << currCluster->GetControlPointIndicesCount() << std::endl;
            //for(int i=0;i<currCluster->GetControlPointIndicesCount();i++){
            //    std::cout << rig.Joints[currJointIndex].controlPointIndices[i] << ", " << rig.Joints[currJointIndex].weights[i] << std::endl;
            //}
            //rig.Joints[currJointIndex].controlPoints = currMesh->GetControlPoints();
            for(int i=0;i<currCluster->GetControlPointIndicesCount();i++){
                FbxVector4 ctrlPoint = currMesh->GetControlPoints()[currCluster->GetControlPointIndices()[i]];
                glm::vec3 ctrlPointConvert;
                ctrlPointConvert.x = ctrlPoint[0];
                ctrlPointConvert.y = ctrlPoint[1];
                ctrlPointConvert.z = ctrlPoint[2];
                
                Joint_Weight joint_weight(currCluster->GetControlPointWeights()[i], currJointIndex);
                map.insert(std::make_pair(ctrlPointConvert, joint_weight));
            }
            //std::cout << rig.Joints.size() << " vs " << numOfClusters << std::endl;
            
            //Only supports one animation
            FbxAnimStack* currAnimStack = scene->GetCurrentAnimationStack();
            FbxString animStackName = currAnimStack->GetName();
            FbxTakeInfo * takeInfo = scene->GetTakeInfo(animStackName);
            FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
            FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
            
            //std::cout << start.GetFrameCount() << std::endl;
            //std::cout << end.GetFrameCount() << std::endl;
            node->EvaluateGlobalTransform(start);
            for(FbxLongLong i = start.GetFrameCount();i<end.GetFrameCount();i++){
                FbxTime currTime;
                currTime.SetFrame(i);
                FbxAMatrix temp = currCluster->GetLink()->EvaluateGlobalTransform(currTime);
                //PrintMatrix(temp);
                glm::mat4 temp2;
                for(int j=0;j<4;j++){
                    for(int k=0;k<4;k++){
                        temp2[j][k] = temp.Get(j, k);
                    }
                }
                //std::cout << glm::to_string(temp2) << std::endl;
                rig.Joints[currJointIndex].frames.push_back(temp2);
                //rig.Joints[currJointIndex].frames.push_back(glm::mat4(1.0));
                //std::cout << rig.Joints[currJointIndex].frames[i][0][0] << std::endl;
                //PrintMatrix(rig.Joints[currJointIndex].frames[i]);
            }
            //std::cout << mAnimationLength << std::endl;
            
            /*
            // Get animation information
            // Now only supports one take
            FbxAnimStack* currAnimStack = mFBXScene->GetSrcObject(0);
            FbxString animStackName = currAnimStack->GetName();
            mAnimationName = animStackName.Buffer();
            FbxTakeInfo* takeInfo = mFBXScene->GetTakeInfo(animStackName);
            FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
            FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
            mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
            Keyframe** currAnim = &mSkeleton.mJoints[currJointIndex].mAnimation;
            
            for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
            {
                FbxTime currTime;
                currTime.SetFrame(i, FbxTime::eFrames24);
                *currAnim = new Keyframe();
                (*currAnim)->mFrameNum = i;
                FbxAMatrix currentTransformOffset = inNode->EvaluateGlobalTransform(currTime) * geometryTransform;
                (*currAnim)->mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);
                currAnim = &((*currAnim)->mNext);
            }
             */
        }
    }
    
}

FBXModel::FBXModel(const char* lFilename, std::vector<Vertex> * vertices){
    FbxManager* lSdkManager = FbxManager::Create();
    
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
    
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
    
    if(!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FBXModel::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }
    
    scene = FbxScene::Create(lSdkManager,"myScene");
    lImporter->Import(scene);
    FbxGeometryConverter geometryConverter(lSdkManager);
    geometryConverter.Triangulate(scene, true);
    lImporter->Destroy();
    
    FbxNode* rootNode = scene->GetRootNode();
    if(rootNode) {
        ProcessSkeletonHierarchy(rootNode);
        std::cout << rootNode->GetChildCount() << std::endl;
        for(int i = 0; i < rootNode->GetChildCount(); i++){
            std::cout << "i is " << i << std::endl;
            ProcessMesh(rootNode->GetChild(i), vertices);
            ProcessJointsAndAnimation(rootNode->GetChild(i));
        }
    }
    this->vertices = vertices;
    
    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();
}

void FBXModel::SetGlobalBindInverseMatrices(Shader &ourShader){
    std::vector<glm::mat4> globalBindInverseMatrices;
    for(int i=0;i<rig.Joints.size();i++){
        globalBindInverseMatrices.push_back(rig.Joints[i].globalBindPoseInverse);
    }
    ourShader.setMultipleMat4("JointGlobalBindInverse", rig.Joints.size(), &globalBindInverseMatrices[0]);
}

void FBXModel::updateAnimation(Shader &ourShader, int frameIndex){
    //i is the index of the frame.
    std::vector<glm::mat4> jointTransformations;
        //Get joint transformations
    for(int j=0;j<rig.Joints.size();j++){
        jointTransformations.push_back(rig.Joints[j].frames[frameIndex]);
    }
    ourShader.setMultipleMat4("Animation",rig.Joints.size(), &jointTransformations[0]);
}

typename std::multimap<glm::vec3, Joint_Weight>::const_iterator find_pair(std::multimap<glm::vec3, Joint_Weight>& map, std::pair<glm::vec3, Joint_Weight>& pair)
{
    typedef typename std::multimap<glm::vec3, Joint_Weight>::const_iterator it;
    std::pair<it,it> range = map.equal_range(pair.first);
    for (it p = range.first; p != range.second; ++p)
        if ((p->second.JointIndex == pair.second.JointIndex) && (p->second.weight == pair.second.weight))
            return p;
    return map.end();
}
bool insert_if_not_present(std::multimap<glm::vec3, Joint_Weight>& map, std::pair<glm::vec3, Joint_Weight>& pair)
{
    if (find_pair(map, pair) == map.end()) {
        map.insert(pair);
        return true;
    }
    return false;
}

void FBXModel::SetJointIndices_Weights(Shader& outShader){
    std::multimap<glm::vec3, Joint_Weight> removeDuplicatePair;
    for (std::map<glm::vec3, Joint_Weight>::iterator it=map.begin(); it!=map.end(); ++it){
        std::pair<glm::vec3, Joint_Weight> pair = std::make_pair(it->first, it->second);
        insert_if_not_present(removeDuplicatePair, pair);
    }
    for(int i=0;i<vertices->size();i++){
        /*
        glm::vec3 position = (*vertices)[i].position;
        std::pair<std::multimap<glm::vec3, Joint_Weight>::iterator, std::multimap<glm::vec3, Joint_Weight>::iterator> result = removeDuplicatePair.equal_range(position);
        std::cout << "All values for this position are" << std::endl;
        for(std::multimap<glm::vec3, Joint_Weight>::iterator it=result.first;it!=result.second;it++){
            Joint_Weight temp = it->second;
            std::cout << temp.JointIndex << ", and weight is " << temp.weight << std::endl;
        }
        
        int count = std::distance(result.first, result.second);
        std::cout << "Total number of values for key are " << count << std::endl;
         */
        float indices[4] = {};
        float weights[4] = {};
        float indices2[4] = {};
        float weights2[4] = {};
        
        glm::vec3 position = (*vertices)[i].position;
        std::pair<std::multimap<glm::vec3, Joint_Weight>::iterator, std::multimap<glm::vec3, Joint_Weight>::iterator> result = removeDuplicatePair.equal_range(position);
        int count = 0;
        for(std::multimap<glm::vec3, Joint_Weight>::iterator it=result.first;it!=result.second;it++){
            Joint_Weight temp = it->second;
            if(count < 4){
                indices[count] = temp.JointIndex;
                weights[count] = temp.weight;
            }
            else{
                indices2[count-4] = temp.JointIndex;
                weights2[count-4] = temp.weight;
            }
            count++;
        }
        (*vertices)[i].jointIndices = glm::vec4(indices[0], indices[1], indices[2], indices[3]);
        (*vertices)[i].jointIndices2 = glm::vec4(indices2[0], indices2[1], indices2[2], indices2[3]);
        
        (*vertices)[i].weights = glm::vec4(weights[0], weights[1], weights[2], weights[3]);
        (*vertices)[i].weights2 = glm::vec4(weights2[0], weights2[1], weights2[2], weights2[3]);
        
    }
}
