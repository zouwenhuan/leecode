//#define METHOD_0
#define METHOD_1

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

#ifdef METHOD_0
int getMinDepth(struct TreeNode* root, int curDepth)
{
    int ld = 0;
    int rd = 0;
    int cd = 0;
    if(root != NULL)
    {
        ld = getMinDepth(root->left, curDepth + 1);
        rd = getMinDepth(root->right, curDepth + 1);
        if(ld == 0 || rd == 0)
            cd = ld + rd;
        else
            cd = ld < rd ? ld : rd;
        cd += 1;
    }
    return cd;
}
#endif //METHOD_0

#ifdef METHOD_1

#define MAX_QUEUE_SIZE 1024
static struct TreeNode** gqueue = NULL;
static int gqueueSize = 0;
static int gqueueHead = 0;
static int gqueueTail = 0;
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


int getMinDepth(struct TreeNode* root, int curDepth)
{
    struct TreeNode* node;
    int found = 0;
    int depth = 0;
    if(root == NULL)
        return 0;
    
    initQueue();
    queuePush(root);
    while(found == 0)
    {
        int layerSize = getQueueSize();
        int i;
        for(i = 0; i < layerSize; i++)
        {
            node = queuePop();
            if(node->left == NULL && node->right == NULL)
            {
                found = 1;
                break;
            }
            else
            {
                if(node->left != NULL)
                    queuePush(node->left);
                if(node->right != NULL)
                    queuePush(node->right);
            }
        }
        depth++;
    }
    destroyQueue();
    return depth;
}
#endif //METHOD_1

int minDepth(struct TreeNode* root){
    return getMinDepth(root, 0);
}