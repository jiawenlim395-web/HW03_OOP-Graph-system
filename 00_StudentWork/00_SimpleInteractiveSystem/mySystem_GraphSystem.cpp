//********************************************
// Student Name			:Lim Jia Wen 
// Student ID			:114550112
// Student Email Address:jiawen06.cs14@nycu.edu.tw
//********************************************
//
//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Yang Ming Chiao Tung University, Taiwan
// Computer Science
// Date: 2026/02
//
//
#include <iostream>
#include "mySystem_GraphSystem.h"
#include <time.h>

using namespace std;

namespace SYS_CONSTANTS {
     int max_int = INT_MAX;
     double max_double = DBL_MAX;
};

int Param::GRAPH_MAX_NUM_NODES = 10000;
int Param::GRAPH_MAX_NUM_EDGES = 10000;

GRAPH_SYSTEM::GRAPH_SYSTEM( )
{
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    mOperation = GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE;
    mFlgShowNodeDepth = true;

    mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
    mFlgShowNodeDepth = true;

    mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
    mOperationName = "SHORTEST_PATH";

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;

    initMemoryPool();
    createDefaultGraph();
    mNumPoints_DoubleCircles = 24;
    // modify and add your code heres
    
    

}

void GRAPH_SYSTEM::initMemoryPool( )
{
    mMaxNumNodes = Param::GRAPH_MAX_NUM_NODES;
    mMaxNumEdges = Param::GRAPH_MAX_NUM_EDGES;

    mNodeArr_Pool = new GRAPH_NODE[mMaxNumNodes];
    mEdgeArr_Pool = new GRAPH_EDGE[mMaxNumEdges];

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;
    mActiveNodeArr = new int[mMaxNumNodes];
    mActiveEdgeArr = new int[mMaxNumEdges];

    mFreeNodeArr = new int[mMaxNumNodes];
    mFreeEdgeArr = new int[mMaxNumEdges];
    //
    for ( int i = 0; i < mMaxNumNodes; ++i ) {
        mNodeArr_Pool[ i ].id = i;
    }
    for ( int i = 0; i < mMaxNumEdges; ++i ) {
        mEdgeArr_Pool[ i ].id = i;
    }
    //
    reset( );
}

void GRAPH_SYSTEM::reset( )
{
    stopAutoNodeDeletion();

    //mNumPoints_DoubleCircles = 0;

    mCurNumOfActiveNodes = 0;
    mCurNumOfActiveEdges = 0;

    mCurNumOfFreeNodes = mMaxNumNodes;
    mCurNumOfFreeEdges = mMaxNumEdges;

    for ( int i = 0; i < mCurNumOfFreeNodes; ++i ) {
        mFreeNodeArr[ i ] = i;
    }
    for ( int i = 0; i < mCurNumOfFreeEdges; ++i ) {
        mFreeEdgeArr[ i ] = i;
    }

    mPassiveSelectedNode = 0;
    mSelectedNode = 0;
    //
    mFlgAutoNodeDeletion = false;

    mFlgShowNodeDepth = false;

    mMaxNodeDepth = 0.0;

    mStartNode = nullptr;
    mDestinationNode = nullptr;
}

GRAPH_NODE *GRAPH_SYSTEM::getFreeNode( ) 
{
    if ( mCurNumOfFreeNodes == 0 ) return 0;
    --mCurNumOfFreeNodes;
    int id = mFreeNodeArr[ mCurNumOfFreeNodes ];
    GRAPH_NODE *n = &mNodeArr_Pool[ id ];
    mActiveNodeArr[ mCurNumOfActiveNodes ] = id;
    n->dynamicID = mCurNumOfActiveNodes;
    ++mCurNumOfActiveNodes;
    return n;
}

GRAPH_EDGE *GRAPH_SYSTEM::getFreeEdge( )
{
    if ( mCurNumOfFreeEdges == 0 ) return 0;
    --mCurNumOfFreeEdges;
    int id = mFreeEdgeArr[ mCurNumOfFreeEdges ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ id ];
    mActiveEdgeArr[ mCurNumOfActiveEdges ] = id;
    e->dynamicID = mCurNumOfActiveEdges;

    ++mCurNumOfActiveEdges;
    return e;
}

void GRAPH_SYSTEM::createDefaultGraph( )
{
    cout << "here"<< endl;
    reset( );

    float offset_x = 50.;
    float offset_z = 30.;

    int n_0 = addNode(offset_x + 0.0, 0.0, offset_z + 0.0 , 1.0);
    cout << "n_0:"<< n_0 << endl;

    //
    // modify and add your code heres
    //
    int n_1 = addNode(offset_x + 20.0, 0.0, offset_z + 0.0, 1.0);
    int n_2 = addNode(offset_x + 0.0, 0.0, offset_z + 20.0, 1.0);

    addEdge( n_0, n_1 );
    addEdge(n_1, n_2);
    
    return;
}

void GRAPH_SYSTEM::createRandomGraph_DoubleCircles(int n)
{
    reset();
    vector <int> inner_nodes;
    vector <int> outer_nodes;

    //n = 36;
    float dx = 30.0;
    float dz = 30.0;
    float r = 15; // radius
    float d = 20; // layer distance
    float offset_x = 40.;
    float offset_z = 60.;
    // modify and add your code heres

    float inner_radius = r;
    float outer_radius = r + d;
    float angle_step = 2.0 * 3.1415926 / n;

    for (int i = 0; i < n; i++) {
        float theta = i * angle_step;
        float xi = offset_x + inner_radius * cos(theta);
        float zi = offset_z + inner_radius * sin(theta);
        float xo = offset_x + outer_radius * cos(theta);
        float zo = offset_z + outer_radius * sin(theta);
        float y = 0;

        //add inner node
        inner_nodes.push_back(addNode(xi, y, zi, 1.0));
        //add outer node
        outer_nodes.push_back(addNode(xo, y, zo, 1.0));

    }

    //connect inner circle
    for (int i = 0; i < n; i++) {
        addEdge(inner_nodes[i], inner_nodes[(i + 1) % n]);//last one back to the first one
    }

    //connect outer circle
    for (int r = 0; r < n; r++) {
        addEdge(outer_nodes[r], outer_nodes[(r + 1) % n]);
    }
    
    //connect inner to closed outer微偏移
    for (int p = 0; p < n; p++) {
        int offset = rand() % 5 - 2;
        int k = p + offset;
        k = (k + n) % n;
        addEdge(inner_nodes[p], outer_nodes[k]);
    }
    return;
}

void GRAPH_SYSTEM::createNet_Circular( int n, int num_layers )
{
    reset( );

    float dx = 10.0;
    float dz = 10.0;
    float r = 5; // radius
    float d = 5; // layer distance 
    float offset_x = 40.;
    float offset_z = 60.;

    // modify and add your code heres

    float angle_step = 2 * 3.1415926 / n;
    vector<vector<int>>layernode(num_layers + 1, vector<int>(n, -1));

    for (int p = 0; p < num_layers + 1; p++) {
        float radius = r + p * d;
        for (int i = 0; i < n; i++) {

            float theta = i * angle_step;

            float x = offset_x + radius * cos(theta);
            float z = offset_z + radius * sin(theta);

            layernode[p][i] = addNode(x, 0, z, 1.0);

        }
    }
    
    //connect inner and outer layer
    for (int p = 0; p < num_layers; p++) {
        for (int i = 0; i < n; i++) {
            addEdge(layernode[p][i], layernode[p + 1][i]);
        }
    }

    //connect to be a circled
    for (int p = 0; p < num_layers; p++) {
        for (int i = 0; i < n ; i++) {
            addEdge(layernode[p][i], layernode[p][(i+1)%n]);
        }
    }

    return;
}

void GRAPH_SYSTEM::createNet_Square( int n, int num_layers )
{
    reset( );

    float dx = 6.0;
    float dz = 6.0;
    float r = 5; // radius
    float d = 5; // layer distance 
    float offset_x = 4.;
    float offset_z = 9.;
    //
    // modify and add your code heres
    //

    int side_length = 2 * num_layers + n;
    //initialize -1
    vector<vector<int>>node(side_length, vector<int>(side_length, -1));
    
    for (int row = 0; row < side_length; row++) {

        float nz = offset_z + row * dz;

        for (int column = 0; column < side_length; column++) {

            float nx = offset_x + column * dx;
            bool insidehole = row >= num_layers &&
                row <= num_layers + n -2 &&
                column <= n + num_layers - 2 &&
                column >= num_layers;

            if (insidehole) {
                continue;
            }
            node[row][column] = addNode(nx, 0, nz, 1.0);
        }
    }
    for (int row = 0; row < side_length; row++) {
        for (int col = 0; col < side_length; col++) {

            int current = node[row][col];
            
            if (current == -1) {
                continue;
            }
            else {
                if (col + 1 < side_length) {
                    int right = node[row][col + 1];
                    if (right != -1) {
                        addEdge(current, right);
                    }
                }

                if(row + 1 < side_length){
                    int down = node[row + 1][col];
                    if (down != -1) {
                        addEdge(current, down);
                    }
                } 
            }
        }
    }

}

void GRAPH_SYSTEM::createNet_RadicalCircular( int n ) {

    reset( );

    float offset_x = 40.0;
    float offset_z = 60.0;

    float r = 25; // radius

    //
    // modify and add your code heres
    //
    float angle_step = 2 * 3.1415926 / n;
    int inner_node;
    vector<int> outer_node;

    for (int i = 0; i < n; i++) {
        float theta = i * angle_step;
        float xo = offset_x + r * cos(theta);
        float zo = offset_z + r * sin(theta);

        //circle
        outer_node.push_back(addNode(xo, 0, zo, 1.0));
      
    }
    inner_node = addNode(offset_x, 0, offset_z, 1.0);
    for (int i = 0; i < n; i++) {
        addEdge(outer_node[i], inner_node);
    }
    return;

}

//
// return the node id
// return -1: no free node
//
int GRAPH_SYSTEM::addNode( float x, float y, float z, float r )
{
    GRAPH_NODE *g;
    g = getFreeNode( );
    //
    // modify and add your code heres
    //

    if (g == nullptr) {
        return -1;
    }
    g->p = vector3(x, y, z);
    g->r = r;
    g->edgeID.clear();
    g->depth = 0;
    g->visited = 0;
    g->path_cost = 0;
    g->path_parent = nullptr;

    return g->id;
}

//
// return the edge id
// return -1: no free edge
//
int GRAPH_SYSTEM::addEdge( int nodeID_0, int nodeID_1 )
{
    GRAPH_EDGE *e;
    e = getFreeEdge( );
    //
    // modify and add your code heres
    //
    if (e == nullptr) {
        return -1;
    }
    e->nodeID[0] = nodeID_0;
    e->nodeID[1] = nodeID_1;

    GRAPH_NODE* n0;
    GRAPH_NODE* n1;
    n0 = &mNodeArr_Pool[nodeID_0];
    n1 = &mNodeArr_Pool[nodeID_1];
    n0->edgeID.push_back(e->id);
    n1->edgeID.push_back(e->id);
    
    return e->id;


}

void GRAPH_SYSTEM::askForInput( )
{
    //
    // modify and add your code heres
    //
    cout << "GRAPH_SYSTEM" << endl;
    cout << "Key usage:" << endl;
    cout << "1: create a default graph" << endl;
    cout << "2: create a graph ..." << endl;
    cout << "3: create a graph ..." << endl;
    cout << "4: create a graph ..." << endl;
    cout << "5: create a graph ..." << endl;
    cout << "Delete: delete a selected node and all the edges incident to it" << endl;
    cout << "Spacebar: Change operation" << endl;
    cout << " " << endl;
    cout << "Use the mouse to select nodes and add edges" << endl;
    cout << "Click the left button to select/unselect or create an edge" << endl;
    cout << " " << endl;
    cout << "A selected node is highlighted as red." << endl;

}


GRAPH_NODE *GRAPH_SYSTEM::findNearestNode( double x, double z, double &cur_distance2 ) const
{

// modify and add your code heres

    GRAPH_NODE *n = nullptr;
    
    cur_distance2 = -1.0;
    int numNodes = getNumOfNodes();
    double dx = 0;
    double dz = 0;
    double d2 = 0;

    for (int i = 0; i < numNodes; i++) {
        int near_idx = mActiveNodeArr[i];
        GRAPH_NODE* near_node = &mNodeArr_Pool[near_idx];
        dx = x - near_node->p.x;
        dz = z - near_node->p.z;
        d2 = dx * dx + dz * dz;

        //check whether it has found or not
        if (cur_distance2 < 0 || d2 < cur_distance2) {
            cur_distance2 = d2;
            n = near_node;
        }
    }

    return n;
}

void GRAPH_SYSTEM::moveTo(double x, double y )
{
    if (!mSelectedNode) return;
    mSelectedNode->p.x = x;
    mSelectedNode->p.z = y;
}

//
// handle the mouse click event at position (x, z).
//
void GRAPH_SYSTEM::clickAt(double x, double z)
{
    double cur_d2;
    GRAPH_NODE *nearest_node = findNearestNode( x, z, cur_d2 );
    if (nearest_node == 0 ) {
        mSelectedNode = 0;
        return;
    }
    if ( cur_d2 > nearest_node->r* nearest_node->r ) {
        mSelectedNode = 0;
        return;

    }
    if ( mSelectedNode == nearest_node) {
        mSelectedNode = 0;
        return;
    }

    performOperation(nearest_node);

}

void GRAPH_SYSTEM::performOperation(GRAPH_NODE* node) {
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mSelectedNode = node;
        mFlgShowNodeDepth = true;
        computeDepthOfAllNodesFromSelectedNode();
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        if (mSelectedNode != 0) {
            addEdge(mSelectedNode->id, node->id);
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        if (mSelectedNode != 0) {
            mStartNode = mSelectedNode;
            mDestinationNode = node;
            computeShortestPath();
            mSelectedNode = 0;
            return;
        }
        mSelectedNode = node;
        break;
    }
}

void GRAPH_SYSTEM::deleteEdge( int edgeID )
{
    if (mCurNumOfActiveEdges <= 0) {
        return;
    }

    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    int dynamicID = e->dynamicID;

    int lastActiveid = mActiveEdgeArr[mCurNumOfActiveEdges - 1];
    mActiveEdgeArr[dynamicID] = lastActiveid;
    mEdgeArr_Pool[lastActiveid].dynamicID = dynamicID;

    mCurNumOfActiveEdges--;

    mFreeEdgeArr[mCurNumOfFreeEdges] = edgeID;
    mCurNumOfFreeEdges++;




    // modify and add your code heres

}

void GRAPH_SYSTEM::removeEdgeFromNode( const GRAPH_EDGE *e, int nodeID )
{

    // modify and add your code heres
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    int sizeofEdgeId = n->edgeID.size();
    int removeedgeID = e->id;

    for (auto it = n->edgeID.begin (); it != n->edgeID.end(); it++) {
        if (*it == e->id) {
            n->edgeID.erase(it);
            break;// found and removed, exit out cleanly
        }
    }
   
}
void GRAPH_SYSTEM::deleteEdgesOfNode( int nodeID )
{ 
    // modify and add your code here
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
   
    for (int i = 0; i < n->edgeID.size(); i++) {

        GRAPH_EDGE* e = &mEdgeArr_Pool[n->edgeID[i]];
        int value = e->nodeID[0];
   
        if (value != n->edgeID[i]) {
            e->nodeID[1] = 0;
        }
        else {
            e->nodeID[0] = 0;
        }
        //deleteEdge(n->edgeID.back());
        n->edgeID.pop_back();
    }

}

void GRAPH_SYSTEM::deleteNode( int nodeID ) {
    if ( mCurNumOfActiveNodes <= 0 ) return;
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
  
    // modify and add your code heres
    
    int new_dynamicId = n->dynamicID;
    int num = mActiveNodeArr[mCurNumOfActiveNodes - 1];
    mNodeArr_Pool[num].dynamicID = new_dynamicId;
    mActiveNodeArr[new_dynamicId] = num;
    mCurNumOfActiveNodes--;

    mFreeNodeArr[mCurNumOfFreeNodes] = nodeID;

    mCurNumOfFreeNodes++;

    return;

}

void GRAPH_SYSTEM::deleteSelectedNode(  ) {
    if ( mSelectedNode == 0 ) return;
    //
    // modify and add your code heres
    
    int id = mSelectedNode->id;
    deleteEdgesOfNode(id);
    deleteNode(id);
    

    mSelectedNode = 0;

    
}

bool GRAPH_SYSTEM::isSelectedNode( ) const
{
    return mSelectedNode != 0;
}

void GRAPH_SYSTEM::getInfoOfSelectedPoint( double &r, vector3 &p ) const
{
    if ( isSelectedNode( ) == false ) return;
    r = mSelectedNode->r;
    p = mSelectedNode->p;

}

void GRAPH_SYSTEM::changeOperation()
{
    switch (mOperation) {
    case GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE;
        mOperationName = "ADD_EDGE";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_ADD_EDGE:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH;
        mOperationName = "SHORTEST_PATH";
        break;
    case GRAPH_OPERATION::GRAPH_OPERATION_SHORTEST_PATH:
        mOperation = GRAPH_OPERATION::GRAPH_OPERATION_NODE_DEPTH;
        mOperationName = "NODE_DEPTH";

        break;
    }
}

void GRAPH_SYSTEM::resetDepthOfAllNodes()
{
    mMaxNodeDepth = 0;
   
    // modify and add your code heres


    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
        n->depth = 0;
        n->path_parent = nullptr;
        n->visited = false;
        
        // modify and add your code heres
         
        // set node's depth
        // and others if necessary
        //
    }
}



/*
* k := computeDepthOfAllNodesFromSelectedNode

* void k( Node *n, int depth ) {
	if n is null, return
	set n’s depth to depth
	for each adjacent node m of n				; note m should not be n
		if (m’s depth < depth + 1) k(m, depth+1)
}

void computeDepthOfAllNodesFromSelectedNode( ) {
	for each node n, set its depth to FLT_MAX
	k(selected_node, 0)
	update mMaxNodeDepth
}
*/

//update all nodes depth from the starting pt
void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode(GRAPH_NODE* node, int depth)
{
    if (node == 0) return;
    node->depth = depth;
    node->visited = true;

        // for all edges incident to the node: node
        
        // determine the next node, which is set as the current node
        // update the current node's depth
        // recursively update all the nodes connected to the current node
        //     
    for (int i = 0; i < node->edgeID.size(); ++i) {
        int edgeid = node->edgeID[i];
        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeid];
        int nextid;
        if (node->id == e->nodeID[0]) {
            nextid = e->nodeID[1];
        }
        else {
            nextid = e->nodeID[0];
        }

        GRAPH_NODE* next = &mNodeArr_Pool[nextid];
        if (!next->visited) {
            computeDepthOfAllNodesFromSelectedNode(next,depth + 1);
        }
    }
            
}

void GRAPH_SYSTEM::computeDepthOfAllNodesFromSelectedNode()
{
    // reset the depth of all nodes
    // if mSelectedNode is nullpute, return
    // set  mMaxNodeDepth = 0;
    // set mSelectedNode->depth as zero
    // invoke computeDepthOfAllNodesFromSelectedNode
    // finally, determine the mMaxNodeDepth
    // 
    
    //
    // modify and add your code heres
    //
    resetDepthOfAllNodes();
    if (mSelectedNode == nullptr) {
        return;
    }
    mMaxNodeDepth = 0;
    mSelectedNode->depth = 0;

    //start from mSelectedNode
    computeDepthOfAllNodesFromSelectedNode(mSelectedNode, 0);

    // Determine the mMaxNodeDepth
    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeid = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeid];
        if (n->depth > mMaxNodeDepth) {
            mMaxNodeDepth = n->depth;
        }

    }
}

// This member function is not used.
float GRAPH_SYSTEM::getNodeDepthFromSelectedNode(int nodeIndex) const
{
    float d = 0.0;
    // modify and add your code heres
    int nodeid = mActiveNodeArr[nodeIndex];
    GRAPH_NODE* node = &mNodeArr_Pool[nodeid];
    d = node->depth;
    return d;
}

void GRAPH_SYSTEM::resetPathInformationOfAllNodes()
{
    
    // modify and add your code heres

    int numNodes = getNumOfNodes();
    for (int i = 0; i < numNodes; ++i) {
        int nodeID = mActiveNodeArr[i];
        GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
  
        // modify and add your code heres

        // set path cost of node
        // set path_parent of node
        n->path_cost = DBL_MAX;
        n->path_parent = nullptr;
    }
}

void GRAPH_SYSTEM::computeShortestPath(GRAPH_NODE *node)
{
    if (node == 0) return;
    // modify and add your code heres
    // for each edge incident to node, node 
    // DO
    for (int i = 0; i < node->edgeID.size(); ++i) {

        // get an edge
        int edgeid = node->edgeID[i];
        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeid];

        // determine the next node. Set it as the current node.
        int next;
        if (node->id == e->nodeID[0]) {
            next = e->nodeID[1];
        }
        else {
            next = e->nodeID[0];
        }
        GRAPH_NODE* cur = &mNodeArr_Pool[next];

        // compute distance d: node->p.distance(next->p);
        double d = node->p.distance(cur->p);

        // if new path cost is not better, check for the other edges
        //length from node to cur's edge
        double new_cost = node->path_cost + d;
        if (new_cost < cur->path_cost) {
            cur->path_cost = new_cost;
            cur->path_parent = node;
            computeShortestPath(cur);
        }
       
        // if new path cost is better, update the node's path cost and path_parent
        
        // Also, invoke computeShortestPath for the current node.
  
        //GRAPH_NODE* n0 = &mNodeArr_Pool[e->nodeID[0]];
        //GRAPH_NODE* n1 = &mNodeArr_Pool[e->nodeID[1]];
        //
        // modify and add your code heres
        //
    }
    
}

void GRAPH_SYSTEM::computeShortestPath()
{
    //
    // modify and add your code heres
    //
    // reset path information of all nodes
    resetPathInformationOfAllNodes();
    if (mStartNode == nullptr || mDestinationNode == nullptr) {
        return;
    }
    mStartNode->path_cost = 0.0;
    computeShortestPath(mStartNode);

    //invokte computeShortestPath with mStartNode
    //
}

void GRAPH_SYSTEM::handleKeyPressedEvent( unsigned char key )
{
    
    switch( key ) {
    case 127: // delete
        mFlgAutoNodeDeletion = false;
        deleteSelectedNode( );
        break;
    case '1':
        mFlgAutoNodeDeletion = false;
        createDefaultGraph( );
        mSelectedNode = 0;
        break;
    case '2':
        mFlgAutoNodeDeletion = false;
        createNet_Circular(25, 6);
        mSelectedNode = 0;

        break;
    case '3':
        mFlgAutoNodeDeletion = false;
        createNet_Square(15, 4);
        mSelectedNode = 0;

        break;
    case '4':
        mFlgAutoNodeDeletion = false;
        createNet_RadicalCircular(24);
        mSelectedNode = 0;

        break;
    case '5':
        mNumPoints_DoubleCircles = 24;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '<':
        mNumPoints_DoubleCircles--;
        if (mNumPoints_DoubleCircles<3) 
            mNumPoints_DoubleCircles = 3;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;
    case '>':
        mNumPoints_DoubleCircles++;
        if (mNumPoints_DoubleCircles> 36) 
            mNumPoints_DoubleCircles = 36;
        mFlgAutoNodeDeletion = false;
        createRandomGraph_DoubleCircles(mNumPoints_DoubleCircles);
        mSelectedNode = 0;
        break;

    case 'r':
        mFlgAutoNodeDeletion = false;
        mSelectedNode = 0;
        break;
    case ' ':
        changeOperation();
        break;
    case 'd':
    case 'D':
        mFlgAutoNodeDeletion = !mFlgAutoNodeDeletion;
        break;
    }
}

void GRAPH_SYSTEM::handlePassiveMouseEvent( double x, double z )
{
    double cur_d2;
    GRAPH_NODE *n = findNearestNode( x, z, cur_d2 );
    if ( n == 0 ) return;
    if ( cur_d2 > n->r*n->r ) {
        mPassiveSelectedNode = 0;
        return;
    }
    mPassiveSelectedNode = n;
}

int GRAPH_SYSTEM::getNumOfNodes( ) const
{
    return mCurNumOfActiveNodes;
}

void GRAPH_SYSTEM::getNodeInfo( int nodeIndex, double &r, vector3 &p ) const
{
    int nodeID = mActiveNodeArr[ nodeIndex ];
    GRAPH_NODE *n = &mNodeArr_Pool[ nodeID ];
    r = n->r;
    p = n->p;
}

void GRAPH_SYSTEM::getNodeInfo(int nodeIndex, double& r, vector3& p, float& depth) const
{
    int nodeID = mActiveNodeArr[nodeIndex];
    GRAPH_NODE* n = &mNodeArr_Pool[nodeID];
    r = n->r;
    p = n->p;
    depth = n->depth;
}

int GRAPH_SYSTEM::getNumOfEdges( ) const
{
    return mCurNumOfActiveEdges;
}

vector3 GRAPH_SYSTEM::getNodePositionOfEdge( int edgeIndex, int nodeIndex ) const
{
    int edgeID = mActiveEdgeArr[ edgeIndex ];
    GRAPH_EDGE *e = &mEdgeArr_Pool[ edgeID ];
    return mNodeArr_Pool[ e->nodeID[ nodeIndex ] ].p;
}

void GRAPH_SYSTEM::stopAutoNodeDeletion()
{
    mFlgAutoNodeDeletion = false;
}

//
// For every frame, update( ) function is called.
//
// 
void GRAPH_SYSTEM::update( )
{
    if (!mFlgAutoNodeDeletion) {
        return;
    }
    if (mCurNumOfActiveNodes<=0) {
     mFlgAutoNodeDeletion = false;
        return;
    }
    Sleep(250);

    //
    // modify and add your code
    // 
    if (mSelectedNode == nullptr) {
        int id = mActiveNodeArr[0];
        mSelectedNode = &mNodeArr_Pool[id];
    }
    GRAPH_NODE* m = mSelectedNode;
    
    //only size changed
    //[2,5,8]delete the first one then 5 will move forward

    vector<int> recordedgeid;
    while(m->edgeID.size()>0) {
        int edgeid = m->edgeID[0];
        GRAPH_EDGE* e = &mEdgeArr_Pool[edgeid];
        int n0 = e->nodeID[0];
        int n1 = e->nodeID[1];
        removeEdgeFromNode(e, n0);
        removeEdgeFromNode(e, n1);
        recordedgeid.push_back(edgeid);
    }
    // delete the selected node?
    // delete all the edges incident to the selected node?
    //
    //delete all node and edge id
    for (int i = 0; i < recordedgeid.size(); i++) {
        deleteEdge(recordedgeid[i]);
    }

    deleteNode(m->id);
    mSelectedNode = 0;
    mPassiveSelectedNode = 0;
}