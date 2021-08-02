#ifndef __ENGINE_DESCRIPTOR_LAYOUT_MANAGER__
#define __ENGINE_DESCRIPTOR_LAYOUT_MANAGER__

#include "Debug.hpp"
#include "VulkanMisc.hpp"
#include <vector>
#include <array>

namespace Ge
{
    class DescriptorLayoutManager
    {
    public:
        bool initialize(VulkanMisc *vM);
        void release();
    private:
        VkDescriptorSetLayout m_descriptorSetLayout;
        VulkanMisc *vulkanM;
    };
}

#endif //__ENGINE_DESCRIPTOR_LAYOUT_MANAGER__