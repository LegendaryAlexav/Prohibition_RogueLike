#pragma once

#include "CoreHeaders.h"

namespace fw
{
    // New Resource type needs to inherit from Resource and define a static category ID method.
    //     For example: static ResourceCategoryIDType GetResourceCategoryID() { return "Mesh"; }

    class Resource
    {
    public:
        virtual ~Resource() {}
    };

    // ResourceManager Usage:
    //     Create a resource and adding it to the manager:
    //         resourceManager.Add<Texture>( "Mailbox", new Texture(...) );
    //     Ask the manager for an existing resource:
    //         Texture* pTexture = resourceManager.Get<Texture>( "Mailbox" );

    using ResourceCategoryIDType = const char*;
    using ResourceIDType = std::string;

    class ResourceManager
    {
    public:
        ResourceManager();
        ~ResourceManager();

        void editor_displayResourceLists();

        template <class Type> void add(ResourceIDType name, Type* pResource)
        {
            m_resources[Type::getResourceCategoryID()][name] = pResource;
        }

        template <class Type> Type* get(ResourceIDType name)
        {
            // If the category doesn't exist or the individual resource doesn't exist, assert.
            // This could return nullptr instead if you want to silently fail.
            if( m_resources.count( Type::getResourceCategoryID() ) == 0 ||
                m_resources[Type::getResourceCategoryID()].count( name ) == 0 )
            {
                assert( false );
            }

            return static_cast<Type*>( m_resources[Type::getResourceCategoryID()][name]);
        }

    protected:
        using ResourceList = std::map<ResourceIDType, Resource*>;
        std::map<ResourceCategoryIDType, ResourceList> m_resources;
    };

} // namespace fw
