#ifndef __ENGINE_TEXTURE_MANAGER__
#define __ENGINE_TEXTURE_MANAGER__

#include "VulkanMisc.hpp"
#include "Debug.hpp"
#include "Textures.hpp"
#include <map>
#include "Descriptor.hpp"

namespace Ge
{
    class TextureManager
    {
    private:
        friend class RenderingEngine;
        bool initiliaze(VulkanMisc *vM);
        void release();        
    public:
        Texture * CreateTexture(const char * path);
		void DestroyTexture(Texture * texture);
    private:
        VulkanMisc *vulkanM;
        std::map<Texture *,Textures *> m_textures;
		Textures * nullTexture;
    public:
        static Descriptor * m_descriptor;
    };
}

#endif //__ENGINE_TEXTURE_MANAGER__