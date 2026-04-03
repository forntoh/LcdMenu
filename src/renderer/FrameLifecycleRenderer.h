#pragma once

#include <stdint.h>

/**
 * @brief Optional renderer interface for buffered frame lifecycle.
 *
 * Renderers that require explicit begin/end frame hooks can expose this
 * extension through MenuRenderer::queryExtension().
 */
class FrameLifecycleRenderer {
  public:
    static uint8_t extensionId() { return 1; }

    virtual ~FrameLifecycleRenderer() {}

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
};
