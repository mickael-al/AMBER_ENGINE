#include "Descriptor.hpp"

namespace Ge
{
    Descriptor::Descriptor(VulkanMisc *vM, VkDescriptorType descriptorType, int baseCount)
    {
        m_count = baseCount;                
        vulkanM = vM;
        m_descriptorType = descriptorType;
        m_DescriptorSetLayout = createVkDescriptorSetLayout(vM,baseCount,descriptorType); //TODO swapchain image X3 a verifier si X1 fonctionne 
        m_DescriptorPool = createVkDescriptorPool(vM,baseCount,descriptorType); 
        m_DescriptorSets = createVkDescriptorSet(vM,m_DescriptorSetLayout,m_DescriptorPool);        
    }

	Descriptor::~Descriptor()
	{
		//destroyVkDescriptorSet(vulkanM,m_DescriptorSets,m_DescriptorPool);
		destroyVkDescriptorPool(vulkanM, m_DescriptorPool);
		destroyVkVkDescriptorSetLayout(vulkanM, m_DescriptorSetLayout);
	}

    VkDescriptorSetLayout Descriptor::getDescriptorSetLayout()
    {
        return m_DescriptorSetLayout;
    }

    VkDescriptorPool Descriptor::getDescriptorPool()
    {
        return m_DescriptorPool;
    }

    std::vector<VkDescriptorSet> Descriptor::getDescriptorSets()
    {
        return m_DescriptorSets;
    }

    void Descriptor::updateCount(VulkanMisc *vM, int count, std::vector<VkDescriptorBufferInfo> bufferInfo)
    {
        if(m_count != count)
        {
            destroyVkDescriptorPool(vulkanM,m_DescriptorPool);
            destroyVkVkDescriptorSetLayout(vulkanM,m_DescriptorSetLayout);
            m_DescriptorSetLayout = createVkDescriptorSetLayout(vM,count,m_descriptorType);
            m_DescriptorPool = createVkDescriptorPool(vM,count,m_descriptorType);
            m_DescriptorSets = createVkDescriptorSet(vM,m_DescriptorSetLayout,m_DescriptorPool);               
        }
        m_count = count;
        VkWriteDescriptorSet descriptorWrites{};
        for(int i = 0; i < vM->str_VulkanSwapChainMisc->str_swapChainImages.size();i++)
        {
            descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites.dstSet = m_DescriptorSets[i];
			descriptorWrites.dstBinding = 0;
			descriptorWrites.dstArrayElement = 0;
			descriptorWrites.descriptorType = m_descriptorType;
			descriptorWrites.descriptorCount = static_cast<uint32_t>(bufferInfo.size());
			descriptorWrites.pBufferInfo = bufferInfo.data();
            vkUpdateDescriptorSets(vM->str_VulkanDeviceMisc->str_device, static_cast<uint32_t>(1), &descriptorWrites, 0, nullptr);
        }
    }

    void Descriptor::updateCount(VulkanMisc *vM, int count, std::vector<VkDescriptorImageInfo> bufferInfo)
    {
        if(m_count != count)
        {
			destroyVkDescriptorSet(vulkanM, m_DescriptorSets, m_DescriptorPool);
            destroyVkDescriptorPool(vulkanM,m_DescriptorPool);
            destroyVkVkDescriptorSetLayout(vulkanM,m_DescriptorSetLayout);
            m_DescriptorSetLayout = createVkDescriptorSetLayout(vM,count,m_descriptorType);
            m_DescriptorPool = createVkDescriptorPool(vM,count,m_descriptorType);
            m_DescriptorSets = createVkDescriptorSet(vM,m_DescriptorSetLayout,m_DescriptorPool); 
			vM->str_VulkanSwapChainMisc->swapChainRecreate->recreatePipeline();
        }
        m_count = count;
        VkWriteDescriptorSet descriptorWrites{};
        for(int i = 0; i < vM->str_VulkanSwapChainMisc->str_swapChainImages.size();i++)
        {
            descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites.dstSet = m_DescriptorSets[i];
			descriptorWrites.dstBinding = 0;
			descriptorWrites.dstArrayElement = 0;
			descriptorWrites.descriptorType = m_descriptorType;
			descriptorWrites.descriptorCount = static_cast<uint32_t>(bufferInfo.size());
			descriptorWrites.pImageInfo = bufferInfo.data();
            vkUpdateDescriptorSets(vM->str_VulkanDeviceMisc->str_device, static_cast<uint32_t>(1), &descriptorWrites, 0, nullptr);
        }
    }

    VkDescriptorPool Descriptor::createVkDescriptorPool(VulkanMisc *vM,int count,VkDescriptorType type)
    {
        VkDescriptorPool m_descriptorPool;
        VkDescriptorPoolSize poolSizes{};
        poolSizes.type = type;
        poolSizes.descriptorCount = static_cast<uint32_t>(vM->str_VulkanSwapChainMisc->str_swapChainImages.size()*count);
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSizes;
        poolInfo.maxSets = static_cast<uint32_t>(vM->str_VulkanSwapChainMisc->str_swapChainImages.size());

        if (vkCreateDescriptorPool(vM->str_VulkanDeviceMisc->str_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            Debug::Error("Echec de la creation d'un descriptor pool");
        }              
        return m_descriptorPool;
    }

    VkDescriptorSetLayout Descriptor::createVkDescriptorSetLayout(VulkanMisc *vM,int count,VkDescriptorType type)
    {
        VkDescriptorSetLayout m_descriptorSetLayout;
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = static_cast<uint32_t>(count);
        uboLayoutBinding.descriptorType = type;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(vM->str_VulkanDeviceMisc->str_device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            Debug::Error("Echec de la creation du descriptor set layout");
        }    
        vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.push_back(m_descriptorSetLayout);
        return m_descriptorSetLayout;
    }

    std::vector<VkDescriptorSet> Descriptor::createVkDescriptorSet(VulkanMisc *vM,VkDescriptorSetLayout descriptorSetLayout,VkDescriptorPool descriptorPool)
    {
        std::vector<VkDescriptorSet> m_descriptorSets; 
        int sizeSC = vM->str_VulkanSwapChainMisc->str_swapChainImages.size();
        std::vector<VkDescriptorSetLayout> layouts(sizeSC, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(sizeSC);
		allocInfo.pSetLayouts = layouts.data();

		m_descriptorSets.resize(sizeSC);
		if (vkAllocateDescriptorSets(vM->str_VulkanDeviceMisc->str_device, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
		{
			Debug::Error("Echec de l'allocation du descriptor sets");			
		}
        return m_descriptorSets;
    }

    void Descriptor::destroyVkDescriptorSet(VulkanMisc *vM, std::vector<VkDescriptorSet> descriptorSets,VkDescriptorPool descriptorPool)
    {
        vkFreeDescriptorSets(vM->str_VulkanDeviceMisc->str_device, descriptorPool, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data());
    }

    void Descriptor::destroyVkDescriptorPool(VulkanMisc *vM,VkDescriptorPool descriptorPool)
    {
        vkDestroyDescriptorPool(vM->str_VulkanDeviceMisc->str_device,descriptorPool,nullptr);
    }

    void Descriptor::destroyVkVkDescriptorSetLayout(VulkanMisc *vM,VkDescriptorSetLayout descriptorSetLayout)
    {
        vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.erase(std::remove(vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.begin(), vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.end(), descriptorSetLayout), vM->str_VulkanSwapChainMisc->str_descriptorSetLayout.end());                
        vkDestroyDescriptorSetLayout(vM->str_VulkanDeviceMisc->str_device, descriptorSetLayout, nullptr);
    }
}