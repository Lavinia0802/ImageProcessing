#include "pch.h"
#include "Filter.h"
#include "Helpers.h"
#include "ImageDocument.h"

IP_NAMESPACE_START

namespace
{
	class GrayscaleFilter final : public ImageFilter
	{
	public:
		void Apply(PixelImage& image) const override
		{
			for (std::size_t index = 0; index < image.pixels.size(); index += 4) {
				const int gray = static_cast<int>(GRAYSCALE_RED_WEIGHT * image.pixels[index] + GRAYSCALE_GREEN_WEIGHT * image.pixels[index + 1] + GRAYSCALE_BLUE_WEIGHT * image.pixels[index + 2]);
				image.pixels[index] = Helpers::ClampByte(gray);
				image.pixels[index + 1] = Helpers::ClampByte(gray);
				image.pixels[index + 2] = Helpers::ClampByte(gray);
			}
		}
	};

	class InvertFilter final : public ImageFilter
	{
	public:
		void Apply(PixelImage& image) const override
		{
			for (std::size_t index = 0; index < image.pixels.size(); index += 4) {
				image.pixels[index] = Helpers::ClampByte(MAX_COLOR_VALUE - image.pixels[index]);
				image.pixels[index + 1] = Helpers::ClampByte(MAX_COLOR_VALUE - image.pixels[index + 1]);
				image.pixels[index + 2] = Helpers::ClampByte(MAX_COLOR_VALUE - image.pixels[index + 2]);
			}
		}
	};

	class SepiaFilter final : public ImageFilter
	{
	public:
		void Apply(PixelImage& image) const override
		{
			for (std::size_t index = 0; index < image.pixels.size(); index += 4) {
				const float red = static_cast<float>(image.pixels[index]);
				const float green = static_cast<float>(image.pixels[index + 1]);
				const float blue = static_cast<float>(image.pixels[index + 2]);
				image.pixels[index] = Helpers::ClampByte(static_cast<int>(red * SEPIA_RED_RED_WEIGHT + green * SEPIA_RED_GREEN_WEIGHT + blue * SEPIA_RED_BLUE_WEIGHT));
				image.pixels[index + 1] = Helpers::ClampByte(static_cast<int>(red * SEPIA_GREEN_RED_WEIGHT + green * SEPIA_GREEN_GREEN_WEIGHT + blue * SEPIA_GREEN_BLUE_WEIGHT));
				image.pixels[index + 2] = Helpers::ClampByte(static_cast<int>(red * SEPIA_BLUE_RED_WEIGHT + green * SEPIA_BLUE_GREEN_WEIGHT + blue * SEPIA_BLUE_BLUE_WEIGHT));
			}
		}
	};

	class BrightnessFilter final : public ImageFilter
	{
	public:
		explicit BrightnessFilter(float amount) : m_amount(amount) {}

		void Apply(PixelImage& image) const override
		{
			for (std::size_t index = 0; index < image.pixels.size(); index += 4) {
				image.pixels[index] = Helpers::ClampByte(static_cast<int>(image.pixels[index] + m_amount));
				image.pixels[index + 1] = Helpers::ClampByte(static_cast<int>(image.pixels[index + 1] + m_amount));
				image.pixels[index + 2] = Helpers::ClampByte(static_cast<int>(image.pixels[index + 2] + m_amount));
			}
		}

	private:
		double m_amount;
	};
}

std::unique_ptr<ImageFilter> ImageFilterFactory::Create(FilterType type, float amount)
{
	switch (type) {
	case FilterType::Grayscale: return std::make_unique<GrayscaleFilter>();
	case FilterType::Invert: return std::make_unique<InvertFilter>();
	case FilterType::Sepia: return std::make_unique<SepiaFilter>();
	case FilterType::Brightness: return std::make_unique<BrightnessFilter>(amount);
	default: throw std::invalid_argument(UNKNOWN_FILTER_ERROR.data());
	}
}

IP_NAMESPACE_END
