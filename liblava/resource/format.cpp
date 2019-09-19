// file      : liblava/resource/format.cpp
// copyright : Copyright (c) 2018-present, Lava Block OÜ
// license   : MIT; see accompanying LICENSE file

#include <liblava/resource/format.hpp>
#include <liblava/base/memory.hpp>

bool lava::format_is_depth(VkFormat format) {

    switch (format) {

        case VK_FORMAT_D16_UNORM:
        case VK_FORMAT_D16_UNORM_S8_UINT:
        case VK_FORMAT_D24_UNORM_S8_UINT:
        case VK_FORMAT_D32_SFLOAT:
        case VK_FORMAT_X8_D24_UNORM_PACK32:
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return true;

        default:
            return false;
    }
}

bool lava::format_is_stencil(VkFormat format) { return format == VK_FORMAT_S8_UINT; }

bool lava::format_is_depth_stencil(VkFormat format) { return format_is_depth(format) || format_is_stencil(format); }

VkImageAspectFlags lava::format_to_aspect_mask(VkFormat format) {

    switch (format) {

        case VK_FORMAT_UNDEFINED:
            return 0;

        case VK_FORMAT_S8_UINT:
            return VK_IMAGE_ASPECT_STENCIL_BIT;

        case VK_FORMAT_D16_UNORM_S8_UINT:
        case VK_FORMAT_D24_UNORM_S8_UINT:
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return VK_IMAGE_ASPECT_STENCIL_BIT | VK_IMAGE_ASPECT_DEPTH_BIT;

        case VK_FORMAT_D16_UNORM:
        case VK_FORMAT_D32_SFLOAT:
        case VK_FORMAT_X8_D24_UNORM_PACK32:
            return VK_IMAGE_ASPECT_DEPTH_BIT;

        default:
            return VK_IMAGE_ASPECT_COLOR_BIT;
    }
}

void lava::format_block_dim(VkFormat format, ui32& width, ui32& height) {

#define fmt(x, w, h)    \
    case VK_FORMAT_##x: \
        width = w;      \
        height = h;     \
        break

    switch (format) {

        fmt(ETC2_R8G8B8A8_UNORM_BLOCK, 4, 4);
        fmt(ETC2_R8G8B8A8_SRGB_BLOCK, 4, 4);
        fmt(ETC2_R8G8B8A1_UNORM_BLOCK, 4, 4);
        fmt(ETC2_R8G8B8A1_SRGB_BLOCK, 4, 4);
        fmt(ETC2_R8G8B8_UNORM_BLOCK, 4, 4);
        fmt(ETC2_R8G8B8_SRGB_BLOCK, 4, 4);
        fmt(EAC_R11_UNORM_BLOCK, 4, 4);
        fmt(EAC_R11_SNORM_BLOCK, 4, 4);
        fmt(EAC_R11G11_UNORM_BLOCK, 4, 4);
        fmt(EAC_R11G11_SNORM_BLOCK, 4, 4);

        fmt(BC1_RGB_UNORM_BLOCK, 4, 4);
        fmt(BC1_RGB_SRGB_BLOCK, 4, 4);
        fmt(BC1_RGBA_UNORM_BLOCK, 4, 4);
        fmt(BC1_RGBA_SRGB_BLOCK, 4, 4);
        fmt(BC2_UNORM_BLOCK, 4, 4);
        fmt(BC2_SRGB_BLOCK, 4, 4);
        fmt(BC3_UNORM_BLOCK, 4, 4);
        fmt(BC3_SRGB_BLOCK, 4, 4);

        fmt(ASTC_4x4_SRGB_BLOCK, 4, 4);
        fmt(ASTC_5x4_SRGB_BLOCK, 5, 4);
        fmt(ASTC_5x5_SRGB_BLOCK, 5, 5);
        fmt(ASTC_6x5_SRGB_BLOCK, 6, 5);
        fmt(ASTC_6x6_SRGB_BLOCK, 6, 6);
        fmt(ASTC_8x5_SRGB_BLOCK, 8, 5);
        fmt(ASTC_8x6_SRGB_BLOCK, 8, 6);
        fmt(ASTC_8x8_SRGB_BLOCK, 8, 8);
        fmt(ASTC_10x5_SRGB_BLOCK, 10, 5);
        fmt(ASTC_10x6_SRGB_BLOCK, 10, 6);
        fmt(ASTC_10x8_SRGB_BLOCK, 10, 8);
        fmt(ASTC_10x10_SRGB_BLOCK, 10, 10);
        fmt(ASTC_12x10_SRGB_BLOCK, 12, 10);
        fmt(ASTC_12x12_SRGB_BLOCK, 12, 12);
        fmt(ASTC_4x4_UNORM_BLOCK, 4, 4);
        fmt(ASTC_5x4_UNORM_BLOCK, 5, 4);
        fmt(ASTC_5x5_UNORM_BLOCK, 5, 5);
        fmt(ASTC_6x5_UNORM_BLOCK, 6, 5);
        fmt(ASTC_6x6_UNORM_BLOCK, 6, 6);
        fmt(ASTC_8x5_UNORM_BLOCK, 8, 5);
        fmt(ASTC_8x6_UNORM_BLOCK, 8, 6);
        fmt(ASTC_8x8_UNORM_BLOCK, 8, 8);
        fmt(ASTC_10x5_UNORM_BLOCK, 10, 5);
        fmt(ASTC_10x6_UNORM_BLOCK, 10, 6);
        fmt(ASTC_10x8_UNORM_BLOCK, 10, 8);
        fmt(ASTC_10x10_UNORM_BLOCK, 10, 10);
        fmt(ASTC_12x10_UNORM_BLOCK, 12, 10);
        fmt(ASTC_12x12_UNORM_BLOCK, 12, 12);

        default:
            width = 1;
            height = 1;
            break;
    }

#undef fmt
}

void lava::format_align_dim(VkFormat format, ui32& width, ui32& height) {

    ui32 align_width, align_height;
    format_block_dim(format, align_width, align_height);
    width = ((width + align_width - 1) / align_width) * align_width;
    height = ((height + align_height - 1) / align_height) * align_height;
}

void lava::format_num_blocks(VkFormat format, ui32& width, ui32& height) {

    ui32 align_width, align_height;
    format_block_dim(format, align_width, align_height);
    width = (width + align_width - 1) / align_width;
    height = (height + align_height - 1) / align_height;
}

lava::ui32 lava::format_block_size(VkFormat format) {

#define fmt(x, bpp)     \
    case VK_FORMAT_##x: \
        return bpp

    switch (format) {

        fmt(R4G4_UNORM_PACK8, 1);
        fmt(R4G4B4A4_UNORM_PACK16, 2);
        fmt(B4G4R4A4_UNORM_PACK16, 2);
        fmt(R5G6B5_UNORM_PACK16, 2);
        fmt(B5G6R5_UNORM_PACK16, 2);
        fmt(R5G5B5A1_UNORM_PACK16, 2);
        fmt(B5G5R5A1_UNORM_PACK16, 2);
        fmt(A1R5G5B5_UNORM_PACK16, 2);
        fmt(R8_UNORM, 1);
        fmt(R8_SNORM, 1);
        fmt(R8_USCALED, 1);
        fmt(R8_SSCALED, 1);
        fmt(R8_UINT, 1);
        fmt(R8_SINT, 1);
        fmt(R8_SRGB, 1);
        fmt(R8G8_UNORM, 2);
        fmt(R8G8_SNORM, 2);
        fmt(R8G8_USCALED, 2);
        fmt(R8G8_SSCALED, 2);
        fmt(R8G8_UINT, 2);
        fmt(R8G8_SINT, 2);
        fmt(R8G8_SRGB, 2);
        fmt(R8G8B8_UNORM, 3);
        fmt(R8G8B8_SNORM, 3);
        fmt(R8G8B8_USCALED, 3);
        fmt(R8G8B8_SSCALED, 3);
        fmt(R8G8B8_UINT, 3);
        fmt(R8G8B8_SINT, 3);
        fmt(R8G8B8_SRGB, 3);
        fmt(R8G8B8A8_UNORM, 4);
        fmt(R8G8B8A8_SNORM, 4);
        fmt(R8G8B8A8_USCALED, 4);
        fmt(R8G8B8A8_SSCALED, 4);
        fmt(R8G8B8A8_UINT, 4);
        fmt(R8G8B8A8_SINT, 4);
        fmt(R8G8B8A8_SRGB, 4);
        fmt(B8G8R8A8_UNORM, 4);
        fmt(B8G8R8A8_SNORM, 4);
        fmt(B8G8R8A8_USCALED, 4);
        fmt(B8G8R8A8_SSCALED, 4);
        fmt(B8G8R8A8_UINT, 4);
        fmt(B8G8R8A8_SINT, 4);
        fmt(B8G8R8A8_SRGB, 4);
        fmt(A8B8G8R8_UNORM_PACK32, 4);
        fmt(A8B8G8R8_SNORM_PACK32, 4);
        fmt(A8B8G8R8_USCALED_PACK32, 4);
        fmt(A8B8G8R8_SSCALED_PACK32, 4);
        fmt(A8B8G8R8_UINT_PACK32, 4);
        fmt(A8B8G8R8_SINT_PACK32, 4);
        fmt(A8B8G8R8_SRGB_PACK32, 4);
        fmt(A2B10G10R10_UNORM_PACK32, 4);
        fmt(A2B10G10R10_SNORM_PACK32, 4);
        fmt(A2B10G10R10_USCALED_PACK32, 4);
        fmt(A2B10G10R10_SSCALED_PACK32, 4);
        fmt(A2B10G10R10_UINT_PACK32, 4);
        fmt(A2B10G10R10_SINT_PACK32, 4);
        fmt(A2R10G10B10_UNORM_PACK32, 4);
        fmt(A2R10G10B10_SNORM_PACK32, 4);
        fmt(A2R10G10B10_USCALED_PACK32, 4);
        fmt(A2R10G10B10_SSCALED_PACK32, 4);
        fmt(A2R10G10B10_UINT_PACK32, 4);
        fmt(A2R10G10B10_SINT_PACK32, 4);
        fmt(R16_UNORM, 2);
        fmt(R16_SNORM, 2);
        fmt(R16_USCALED, 2);
        fmt(R16_SSCALED, 2);
        fmt(R16_UINT, 2);
        fmt(R16_SINT, 2);
        fmt(R16_SFLOAT, 2);
        fmt(R16G16_UNORM, 4);
        fmt(R16G16_SNORM, 4);
        fmt(R16G16_USCALED, 4);
        fmt(R16G16_SSCALED, 4);
        fmt(R16G16_UINT, 4);
        fmt(R16G16_SINT, 4);
        fmt(R16G16_SFLOAT, 4);
        fmt(R16G16B16_UNORM, 6);
        fmt(R16G16B16_SNORM, 6);
        fmt(R16G16B16_USCALED, 6);
        fmt(R16G16B16_SSCALED, 6);
        fmt(R16G16B16_UINT, 6);
        fmt(R16G16B16_SINT, 6);
        fmt(R16G16B16_SFLOAT, 6);
        fmt(R16G16B16A16_UNORM, 8);
        fmt(R16G16B16A16_SNORM, 8);
        fmt(R16G16B16A16_USCALED, 8);
        fmt(R16G16B16A16_SSCALED, 8);
        fmt(R16G16B16A16_UINT, 8);
        fmt(R16G16B16A16_SINT, 8);
        fmt(R16G16B16A16_SFLOAT, 8);
        fmt(R32_UINT, 4);
        fmt(R32_SINT, 4);
        fmt(R32_SFLOAT, 4);
        fmt(R32G32_UINT, 8);
        fmt(R32G32_SINT, 8);
        fmt(R32G32_SFLOAT, 8);
        fmt(R32G32B32_UINT, 12);
        fmt(R32G32B32_SINT, 12);
        fmt(R32G32B32_SFLOAT, 12);
        fmt(R32G32B32A32_UINT, 16);
        fmt(R32G32B32A32_SINT, 16);
        fmt(R32G32B32A32_SFLOAT, 16);
        fmt(R64_UINT, 8);
        fmt(R64_SINT, 8);
        fmt(R64_SFLOAT, 8);
        fmt(R64G64_UINT, 16);
        fmt(R64G64_SINT, 16);
        fmt(R64G64_SFLOAT, 16);
        fmt(R64G64B64_UINT, 24);
        fmt(R64G64B64_SINT, 24);
        fmt(R64G64B64_SFLOAT, 24);
        fmt(R64G64B64A64_UINT, 32);
        fmt(R64G64B64A64_SINT, 32);
        fmt(R64G64B64A64_SFLOAT, 32);
        fmt(B10G11R11_UFLOAT_PACK32, 4);
        fmt(E5B9G9R9_UFLOAT_PACK32, 4);
        fmt(D16_UNORM, 2);
        fmt(X8_D24_UNORM_PACK32, 4);
        fmt(D32_SFLOAT, 4);
        fmt(S8_UINT, 1);
        fmt(D16_UNORM_S8_UINT, 3); // doesn't make sense.
        fmt(D24_UNORM_S8_UINT, 4);
        fmt(D32_SFLOAT_S8_UINT, 5); // doesn't make sense.

        // ETC2
        fmt(ETC2_R8G8B8A8_UNORM_BLOCK, 16);
        fmt(ETC2_R8G8B8A8_SRGB_BLOCK, 16);
        fmt(ETC2_R8G8B8A1_UNORM_BLOCK, 8);
        fmt(ETC2_R8G8B8A1_SRGB_BLOCK, 8);
        fmt(ETC2_R8G8B8_UNORM_BLOCK, 8);
        fmt(ETC2_R8G8B8_SRGB_BLOCK, 8);
        fmt(EAC_R11_UNORM_BLOCK, 8);
        fmt(EAC_R11_SNORM_BLOCK, 8);
        fmt(EAC_R11G11_UNORM_BLOCK, 16);
        fmt(EAC_R11G11_SNORM_BLOCK, 16);

        // BC
        fmt(BC1_RGB_UNORM_BLOCK, 8);
        fmt(BC1_RGB_SRGB_BLOCK, 8);
        fmt(BC1_RGBA_UNORM_BLOCK, 8);
        fmt(BC1_RGBA_SRGB_BLOCK, 8);
        fmt(BC2_UNORM_BLOCK, 16);
        fmt(BC2_SRGB_BLOCK, 16);
        fmt(BC3_UNORM_BLOCK, 16);
        fmt(BC3_SRGB_BLOCK, 16);

        // ASTC
        fmt(ASTC_4x4_SRGB_BLOCK, 16);
        fmt(ASTC_5x4_SRGB_BLOCK, 16);
        fmt(ASTC_5x5_SRGB_BLOCK, 16);
        fmt(ASTC_6x5_SRGB_BLOCK, 16);
        fmt(ASTC_6x6_SRGB_BLOCK, 16);
        fmt(ASTC_8x5_SRGB_BLOCK, 16);
        fmt(ASTC_8x6_SRGB_BLOCK, 16);
        fmt(ASTC_8x8_SRGB_BLOCK, 16);
        fmt(ASTC_10x5_SRGB_BLOCK, 16);
        fmt(ASTC_10x6_SRGB_BLOCK, 16);
        fmt(ASTC_10x8_SRGB_BLOCK, 16);
        fmt(ASTC_10x10_SRGB_BLOCK, 16);
        fmt(ASTC_12x10_SRGB_BLOCK, 16);
        fmt(ASTC_12x12_SRGB_BLOCK, 16);
        fmt(ASTC_4x4_UNORM_BLOCK, 16);
        fmt(ASTC_5x4_UNORM_BLOCK, 16);
        fmt(ASTC_5x5_UNORM_BLOCK, 16);
        fmt(ASTC_6x5_UNORM_BLOCK, 16);
        fmt(ASTC_6x6_UNORM_BLOCK, 16);
        fmt(ASTC_8x5_UNORM_BLOCK, 16);
        fmt(ASTC_8x6_UNORM_BLOCK, 16);
        fmt(ASTC_8x8_UNORM_BLOCK, 16);
        fmt(ASTC_10x5_UNORM_BLOCK, 16);
        fmt(ASTC_10x6_UNORM_BLOCK, 16);
        fmt(ASTC_10x8_UNORM_BLOCK, 16);
        fmt(ASTC_10x10_UNORM_BLOCK, 16);
        fmt(ASTC_12x10_UNORM_BLOCK, 16);
        fmt(ASTC_12x12_UNORM_BLOCK, 16);

        default:
            assert(0 && "Unknown format.");
            return 0;
    }
#undef fmt
}

bool lava::get_supported_depth_format(VkPhysicalDevice physical_device, VkFormat* depth_format) {

    VkFormats depth_formats = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D16_UNORM };

    for (auto& format : depth_formats) {

        VkFormatProperties format_props;
        vkGetPhysicalDeviceFormatProperties(physical_device, format, &format_props);

        if (format_props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {

            *depth_format = format;
            return true;
        }
    }

    return false;
}

VkImageMemoryBarrier lava::image_memory_barrier(VkImage image, VkImageLayout old_layout, VkImageLayout new_layout) {

    return {

        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = 0,
        .dstAccessMask = 0,
        .oldLayout = old_layout,
        .newLayout = new_layout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image,
        .subresourceRange = {},
    };
}

void lava::set_image_layout(device* device, VkCommandBuffer cmd_buffer, VkImage image, VkImageLayout old_image_layout, VkImageLayout new_image_layout,
                            VkImageSubresourceRange subresource_range, VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask) {

    auto barrier = image_memory_barrier(image, old_image_layout, new_image_layout);
    barrier.subresourceRange = subresource_range;

    switch (old_image_layout) {

        case VK_IMAGE_LAYOUT_UNDEFINED:
            barrier.srcAccessMask = 0;
            break;

        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;
        default:
            break;
    }

    switch (new_image_layout) {

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            if (barrier.srcAccessMask == 0)
                barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;

            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;
        default:
            break;
    }

    device->call().vkCmdPipelineBarrier(cmd_buffer, src_stage_mask, dst_stage_mask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void lava::set_image_layout(device* device, VkCommandBuffer cmd_buffer, VkImage image, VkImageAspectFlags aspect_mask, VkImageLayout old_image_layout,
                                            VkImageLayout new_image_layout, VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask) {

    VkImageSubresourceRange subresource_range
    {
        .aspectMask = aspect_mask,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
    };

    set_image_layout(device, cmd_buffer, image, old_image_layout, new_image_layout, subresource_range, src_stage_mask, dst_stage_mask);
}

void lava::insert_image_memory_barrier(lava::device* device, VkCommandBuffer cmd_buffer, VkImage image, VkAccessFlags src_access_mask, VkAccessFlags dst_access_mask,
                                        VkImageLayout old_image_layout, VkImageLayout new_image_layout, 
                                        VkPipelineStageFlags src_stage_mask, VkPipelineStageFlags dst_stage_mask, VkImageSubresourceRange subresource_range) {

    auto barrier = image_memory_barrier(image, old_image_layout, new_image_layout);

    barrier.srcAccessMask = src_access_mask;
    barrier.dstAccessMask = dst_access_mask;
    barrier.subresourceRange = subresource_range;

    device->call().vkCmdPipelineBarrier(cmd_buffer, src_stage_mask, dst_stage_mask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}
