/*
 * author : Shuichi TAKANO
 * since  : Sun Jun 20 2021 03:34:30
 */
#ifndef C0B8F27B_5134_63A8_332D_5F7EC7D8492C
#define C0B8F27B_5134_63A8_332D_5F7EC7D8492C

#include <stdint.h>
#include <array>
#include <vector>
#include <util/queue.h>
#include <util/ring_buffer.h>

namespace spi_buffer
{
    class SPI_Buffer
    {
    public:
        using PixelType = uint16_t;
        using LineBuffer = std::vector<PixelType>;

    public:
        SPI_Buffer();
        
        uint32_t getFrameCounter() const
        {
            return frameCounter_;
        }

        LineBuffer *__not_in_flash_func(getLineBuffer)();
        void __not_in_flash_func(setLineBuffer)(int line, LineBuffer *);
        void __not_in_flash_func(waitForValidLine)();
        void __not_in_flash_func(spi_task)();

    protected:
        
        void allocateBuffers();

    private:
        int lineCounter_ = 0;
        uint32_t frameCounter_ = 0;

        template <class T>
        struct ResultBuffer
        {
            int line{};
            T buffer{};
        };

        using ResultLineBuffer = ResultBuffer<LineBuffer *>;

        static inline constexpr size_t N_BUFFERS = 5; // was 5

        LineBuffer lineBuffers_[N_BUFFERS];
        LineBuffer *currentLineBuffer_{};

        util::Queue<ResultLineBuffer> validLineQueue_{N_BUFFERS};
        util::Queue<LineBuffer *> freeLineQueue_{N_BUFFERS};
    };
}

#endif /* C0B8F27B_5134_63A8_332D_5F7EC7D8492C */ 