/*
    date: 2019-12-23, 20:37
    easy
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
bool myhasPathSum(struct TreeNode* root, int sum){
    bool flag = false;
    if(root != NULL && root->left == NULL && root->right == NULL)
    {
        if(root->val - sum == 0)
            flag = true;
    }    
    else if(root != NULL)
    {
        flag = myhasPathSum(root->left, sum - root->val);
        if(flag == false)
            flag = myhasPathSum(root->right, sum - root->val);
    }
    return flag;
}

bool hasPathSum(struct TreeNode* root, int sum){
    if(root == NULL)
        return false;
    else
        return myhasPathSum(root, sum);
}
#endif //METHOD_0

#ifdef METHOD_1

#define MAX_STACK_SIZE 1024
static struct TreeNode** gstack = NULL;
static int gstackSize = 0;

static int *garrayStack = NULL;
static int garrayStackSize = 0;
#define ARRAY_STATCK_ERROR 0x8ffff;

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

static void initArrayStack(void)
{
    garrayStack = calloc(MAX_STACK_SIZE, sizeof(int));
    garrayStackSize = 0;
}

static void destroyArrayStack(void)
{
    if(garrayStack)
    {
        free(garrayStack);
        garrayStack = NULL;
    }
    garrayStackSize = 0;
}


static void arrayStatckPush(int val)
{
    garrayStack[garrayStackSize] = val;
    garrayStackSize++;
}

static int arrayStackPop(void)
{
    int val = ARRAY_STATCK_ERROR;
    if(garrayStackSize > 0)
    {
        val = garrayStack[garrayStackSize - 1];
        garrayStackSize--;
    }
    return val;
}

static int getArrayStackSize(void)
{
    return garrayStackSize;
}

bool hasPathSum(struct TreeNode* root, int sum){
    bool found = false;
    struct TreeNode* node = NULL;
    int curSum = sum;
    if(root == NULL)
        return false;
    initStack();
    initArrayStack();
    statckPush(root);
    arrayStatckPush(sum - root->val);
    while(found == false && getStackSize() > 0)
    {
        node = stackPop();
        curSum = arrayStackPop();
        if(node->left == NULL && node->right == NULL && curSum == 0)
        {
            found = true;
            break;
        }
        if(node->left)
        {
            statckPush(node->left);
            arrayStatckPush(curSum - node->left->val);
        }
        if(node->right)
        {
            statckPush(node->right);
            arrayStatckPush(curSum - node->right->val);
        }
    }
    destroyStack();
    destroyArrayStack();
    
    return found;
}
#endif //METHOD_1

