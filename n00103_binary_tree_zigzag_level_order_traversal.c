/* time：2019年12月22日 
    使用了两种方法：递归，队列
 */

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
//#define METHOD_0 
#define METHOD_1

#ifdef METHOD_0
#define MAX_DEPTH 2048
static int depthMaxSize[MAX_DEPTH];

static int getDepthAndSize(struct TreeNode* root, int *sizeInCurDepth, int depthOrder)
{
    int ld = 0;
    int rd = 0;
    int cd = 0;
    if(root != NULL)
    {
        sizeInCurDepth[depthOrder]++;
        ld = getDepthAndSize(root->left, sizeInCurDepth, depthOrder + 1);
        rd = getDepthAndSize(root->right, sizeInCurDepth, depthOrder + 1);
        cd = ld > rd ? ld : rd;
        cd += 1;
    }
    return cd;
}

static void getZigzagOrder(struct TreeNode* root, int *sizeInCurDepth, int *maxSizeInCurDepth, int **order, int curDepth)
{
    int size = 0;
    int index = 0;
    if(root == NULL)
        return;
    size = maxSizeInCurDepth[curDepth];
    if((curDepth & 0x1) == 0)
        index = sizeInCurDepth[curDepth];
    else
        index = size - sizeInCurDepth[curDepth] - 1;
    sizeInCurDepth[curDepth]++;
    order[curDepth][index] = root->val;
    getZigzagOrder(root->left, sizeInCurDepth, maxSizeInCurDepth, order, curDepth + 1);
    getZigzagOrder(root->right, sizeInCurDepth, maxSizeInCurDepth, order, curDepth + 1);  
    
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** zigzagLevelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes){
    int *depthSize = NULL;
    int **order = NULL;
    int depth = 0;
    int i;
    memset(depthMaxSize, 0, sizeof(int) * MAX_DEPTH);
    depth = getDepthAndSize(root, depthMaxSize, 0);
    if(depth != 0)
    {
        depthSize = (int *)calloc(depth, sizeof(int));
        order = (int **)calloc(depth, sizeof(int *));
        for(i = 0; i < depth; i++)
        {
            int ds = depthMaxSize[i];
            order[i] = (int *)calloc(ds, sizeof(int));
        }
        getZigzagOrder(root, depthSize, depthMaxSize, order, 0);
    }
    *returnSize = depth;
    *returnColumnSizes = depthSize;
    return order;
}
#endif //METHOD_0

//==================================================================================================================
#ifdef METHOD_1

#define MAX_DEPTH 2048
#define MAX_NODE_NUM 10240
#define MAX_STACK_SIZE 1024
#define MAX_QUEUE_SIZE 1024

static int depthMaxSize[MAX_DEPTH];

static struct TreeNode** gstack = NULL;
static int gstackSize = 0;

static struct TreeNode** gqueue = NULL;
static int gqueueSize = 0;
static int gqueueHead = 0;
static int gqueueTail = 0;

static void initStack(void)
{
    gstack = calloc(MAX_STACK_SIZE, sizeof(struct TreeNode*));
    gstackSize = 0;
}

static void destroyStack(void)
{
    if(gstack)
    {
        free(gstack);
        gstack = NULL;
    }
    gstackSize = 0;
}

static void statckPush(struct TreeNode* node)
{
    gstack[gstackSize] = node;
    gstackSize++;
}

struct TreeNode* stackPop(void)
{
    struct TreeNode* node = NULL;
    if(gstackSize > 0)
    {
        node = gstack[gstackSize - 1];
        gstackSize--;
    }
    return node;
}

static int getStackSize(void)
{
    return gstackSize;
}

static void initQueue(void)
{
    gqueue = calloc(MAX_QUEUE_SIZE, sizeof(struct TreeNode*));
    gqueueSize = 0;  
    gqueueHead = 0;
    gqueueTail = 0;
}

static void destroyQueue(void)
{
    if(gqueue != NULL)
        free(gqueue);
    gqueueSize = 0;
    gqueueHead = 0;
    gqueueTail = 0;
}

static void queuePush(struct TreeNode* node)
{
    if(gqueueSize >= MAX_QUEUE_SIZE)
    {
        // print error info
    }
    gqueue[gqueueHead] = node;
    gqueueHead++;
    gqueueSize++;
    if(gqueueHead >= (MAX_QUEUE_SIZE - 1))
        gqueueHead = 0;
}

static int getQueueSize(void)
{
    return gqueueSize;
}

static struct TreeNode* queuePop(void)
{
    struct TreeNode* node = NULL;
    if(gqueueSize != 0)
    {
        node = gqueue[gqueueTail];
        gqueueSize--;
        gqueueTail++;
        if(gqueueTail >= (MAX_QUEUE_SIZE - 1))
            gqueueTail = 0;
    }
    return node;
}

static int getMaxDethp(struct TreeNode* root, int *ds)
{
    int maxDepth = 0;
    if(root == NULL)
        return 0;

    initQueue();
    queuePush(root);
    
    while(getQueueSize() != 0)
    {
        int layerSize = getQueueSize();
        int i = 0;
        struct TreeNode* node = NULL;
        ds[maxDepth] = layerSize;
        maxDepth++;
        for(i = 0; i < layerSize; i++)
        {
            node = queuePop();
            if(node->left)
                queuePush(node->left);
            if(node->right)
                queuePush(node->right);
        }
    }
    destroyQueue();
    return maxDepth;
}

static void getZigzagOrder(struct TreeNode* root, int *dsReturn, int **order)
{
    int depth = 0;
    if(root == NULL)
        return;

    initQueue();
    queuePush(root);
    while(getQueueSize() != 0)
    {
        int i = 0;
        struct TreeNode* node = NULL;
        int layerSize = getQueueSize();
        for(i = 0; i < layerSize; i++)
        {
            int indexInOrder = ((depth & 0x1) == 0) ? i : (layerSize - 1 - i);
            node = queuePop();
            order[depth][indexInOrder] = node->val;
            if(node->left)
                queuePush(node->left);
            if(node->right)
                queuePush(node->right);
        }
        dsReturn[depth] = layerSize;           
        depth++;
    }
    destroyQueue();       
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** zigzagLevelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes){
    int maxDepth = 0;
    int *depthSize = NULL;
    int **order = NULL;
    
    memset(depthMaxSize, 0, sizeof(int) * MAX_DEPTH);
    maxDepth = getMaxDethp(root, depthMaxSize);
    if(maxDepth != 0)
    {
        int i;
        depthSize = (int *)calloc(maxDepth, sizeof(int));
        order = (int **)calloc(maxDepth, sizeof(int *));  
        for(i = 0; i < maxDepth; i++)
        {
            int ds = depthMaxSize[i];
            order[i] = (int *)calloc(ds, sizeof(int));
        }
        getZigzagOrder(root, depthSize, order);        
    }            
    *returnSize = maxDepth;
    *returnColumnSizes = depthSize;
    return order;
}


#endif //METHOD_1