#include <fs/fsresolver.hpp>

FSNode* resolvePath(const char *path, FSNode* root, FSNode* current)
{
    FSNode *node;

    if (path[0] == '/')
    {
        node = root;
        path++; // Skip the leading '/'
    }
    else
    {
        node = current;
    }

    char component[NAMESIZE];

    while (*path != '\0')
    {
        // Skip repeated slashes
        while (*path == '/')
            path++;

        if (*path == '\0')
            break;

        // Read one path component
        int i = 0;

        while (*path != '\0' && *path != '/')
        {
            if (i < NAMESIZE - 1)
                component[i++] = *path;

            path++;
        }

        component[i] = '\0';
        if (streq(component, "."))
        {
            continue;
        }

        if (streq(component, ".."))
        {
            if (node->parent != nullptr)
                node = node->parent;

            continue;
        }
        // Find this child inside the current node
        node = findNode(component, current, node, false);

        if (node == nullptr)
            return nullptr;
    }

    return node;
}

ParentResult resolveParent(const char* path, FSNode* root, FSNode* current, bool &err) {
    err = false;
    if (path == nullptr || *path == '\0') {
        err = true;
        return {};
    }

    FSNode *node;
    ParentResult result{};
    //print("err = ");
    //print(err ? "true\n" : "false\n");
    if (path[0] == '/')
    {
        node = root;
        path++; // Skip the leading '/'
        if (*path == '\0')
        {
            result.parent = root;
            result.name[0] = '\0';   // No final component
            return result;
        }
    }
    else
    {
        node = current;
    }

    char component[NAMESIZE];
    while (*path != '\0')
    {
        /*print("PATH: ");
        print(path);
        print("\n");*/
        
        // Skip repeated slashes
        while (*path == '/')
            path++;

        // Read one path component
        int i = 0;

        while (*path != '\0' && *path != '/')
        {
            if (i < NAMESIZE - 1)
                component[i++] = *path;

            path++;
        }

        component[i] = '\0';

        /*print("COMPONENT: ");
        print(component);
        print("\n");*/
        if (streq(component, "."))
        {
            continue;
        }

        if (streq(component, ".."))
        {
            if (node->parent != nullptr)
                node = node->parent;

            continue;
        }

        while (*path == '/')
            path++;

        if (*path == '\0') {
            /*print("Last component: '");
            print(component);
            print("'\n");*/

            result.parent = node;

            memcpy(result.name, component, NAMESIZE);
            result.name[NAMESIZE - 1] = '\0';

            /*print("Returning name: '");
            print(result.name);
            print("'\n");*/

            return result;
        }
        
        // Find this child inside the current node
        node = findNode(component, current, node, false);
        //print(intToString(err));
        if (node == nullptr) {
            /*print("The parent directory does not exist\n");*/
            err = true;
            return {};
        }
        
        if (node->type != Folder) {
            err = true;
            return {};
        }
    }
    /*print("COMPONENT: ");
    print(component);*/
    err = true;
    return {};
}


PFSNode* resolvePFSPath(const char* path, PFSNode* root, ATA* disk, PFSNode* current) {
    PFSNode* node;
    if (path[0] == '/')
    {
        node = root;    
        path++; // Skip the leading '/'
    }
    else
    {
        node = current;
    }

    char component[NAMESIZE];

    while (*path != '\0')
    {
        // Skip repeated slashes
        while (*path == '/')
            path++;

        if (*path == '\0')
            break;

        // Read one path component
        int i = 0;

        while (*path != '\0' && *path != '/')
        {
            if (i < NAMESIZE - 1)
                component[i++] = *path;

            path++;
        }

        component[i] = '\0';
        if (streq(component, "."))
        {
            continue;
        }

        if (streq(component, ".."))
        {
            if (node->parent != nullptr)
                node = node->parent;

            continue;
        }
        // Find this child inside the current node
        node = findPFSNode(component, node, disk, false);

        if (node == nullptr)
            return nullptr;
    }

    return node;
}

PFSParentResult resolvePFSParent(const char* path, PFSNode* root, PFSNode* current, ATA* disk, bool &err) {
    err = false;
    if (path == nullptr || *path == '\0') {
        err = true;
        return {};
    }

    PFSNode *node;
    PFSParentResult result{};
    //print("err = ");
    //print(err ? "true\n" : "false\n");
    if (path[0] == '/')
    {
        node = root;
        path++; // Skip the leading '/'
        if (*path == '\0')
        {
            result.parent = root;
            result.name[0] = '\0';   // No final component
            return result;
        }
    }
    else
    {
        node = current;
    }

    char component[NAMESIZE];
    while (*path != '\0')
    {
        /*print("PATH: ");
        print(path);
        print("\n");*/
        
        // Skip repeated slashes
        while (*path == '/')
            path++;

        // Read one path component
        int i = 0;

        while (*path != '\0' && *path != '/')
        {
            if (i < NAMESIZE - 1)
                component[i++] = *path;

            path++;
        }

        component[i] = '\0';

        /*print("COMPONENT: ");
        print(component);
        print("\n");*/
        if (streq(component, "."))
        {
            continue;
        }

        if (streq(component, ".."))
        {
            if (node->parent != nullptr)
                node = node->parent;

            continue;
        }

        while (*path == '/')
            path++;

        if (*path == '\0') {
            /*print("Last component: '");
            print(component);
            print("'\n");*/

            result.parent = node;

            memcpy(result.name, component, NAMESIZE);
            result.name[NAMESIZE - 1] = '\0';

            /*print("Returning name: '");
            print(result.name);
            print("'\n");*/

            return result;
        }
        
        // Find this child inside the current node
        node = findPFSNode(component, node, disk, false);
        //print(intToString(err));
        if (node == nullptr) {
            /*print("The parent directory does not exist\n");*/
            err = true;
            return {};
        }
        
        if (node->type != Folder) {
            err = true;
            return {};
        }
    }
    /*print("COMPONENT: ");
    print(component);*/
    err = true;
    return {};
}
