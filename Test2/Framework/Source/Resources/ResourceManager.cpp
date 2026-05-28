//
// Copyright (c) 2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include "ResourceManager.h"
#include "Imgui/imgui.h"

namespace fw
{
    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
        for( auto& resourceCategoryPair : m_resources )
        {
            for( auto& resource : resourceCategoryPair.second )
            {
                delete resource.second;
            }
        }
    }

    void ResourceManager::editor_displayResourceLists()
    {
        ImGui::Text( "Resources:" );

        for( auto it = m_resources.begin(); it != m_resources.end(); it++ )
        {
            ResourceIDType type = it->first;
            ResourceList& list = it->second;

            ImGui::Text( "  %s:", type.c_str() );

            for( auto it2 = list.begin(); it2 != list.end(); it2++ )
            {
                ImGui::Text( "    %s", it2->first.c_str() );
            }
        }
    }

} // namespace fw
