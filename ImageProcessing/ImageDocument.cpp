#include "pch.h"
#include "ImageDocument.h"

IP_NAMESPACE_START

bool PixelImage::IsValid() const noexcept
{
	return width > 0 && height > 0 && pixels.size() == static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * PIXEL_CHANNEL_COUNT;
}

bool ImageDocument::Load(const std::wstring& path, std::wstring& error)
{
	Gdiplus::Bitmap bitmap(path.c_str());
	if (bitmap.GetLastStatus() != Gdiplus::Ok || bitmap.GetWidth() == 0 || bitmap.GetHeight() == 0) {
		error = Helpers::GdiStatusMessage(bitmap.GetLastStatus());
		return false;
	}

	PixelImage image;
	image.width = static_cast<int>(bitmap.GetWidth());
	image.height = static_cast<int>(bitmap.GetHeight());
	image.pixels.resize(static_cast<std::size_t>(image.width) * static_cast<std::size_t>(image.height) * PIXEL_CHANNEL_COUNT);
	if (!Helpers::ReadBitmapPixels(bitmap, image, error)) return false;

	m_sourcePath = path;
	m_history.clear();
	m_history.push_back(std::move(image));
	m_historyIndex = 0;
	return true;
}

bool ImageDocument::Save(const std::wstring& path, std::wstring& error) const
{
	if (!HasImage()) {
		error = NO_IMAGE_SAVE_ERROR;
		return false;
	}

	Gdiplus::Bitmap bitmap(Current().width, Current().height, PixelFormat32bppARGB);
	if (bitmap.GetLastStatus() != Gdiplus::Ok) {
		error = Helpers::GdiStatusMessage(bitmap.GetLastStatus());
		return false;
	}

	if (!Helpers::WriteBitmapPixels(bitmap, Current(), error)) {
		return false;
	}

	CLSID encoder{};
	if (!Helpers::GetEncoderClsid(Helpers::FileExtension(path), encoder)) {
		error = UNSUPPORTED_OUTPUT_ERROR;
		return false;
	}

	const Gdiplus::Status status = bitmap.Save(path.c_str(), &encoder, nullptr);
	if (status != Gdiplus::Ok) {
		error = Helpers::GdiStatusMessage(status);
		return false;
	}

	return true;
}

void ImageDocument::ResetHistory()
{
	m_history.clear();
	m_historyIndex = 0;
	m_sourcePath.clear();
}

void ImageDocument::ClearImage()
{
	ResetHistory();
}

void ImageDocument::Commit(PixelImage image)
{
	if (!image.IsValid()) {
		return;
	}

	if (m_historyIndex + 1 < m_history.size()) {
		m_history.erase(m_history.begin() + static_cast<std::vector<PixelImage>::difference_type>(m_historyIndex + 1), m_history.end());
	}

	m_history.push_back(std::move(image));
	m_historyIndex = m_history.size() - 1;
}

void ImageDocument::ApplyFilter(FilterType type, float amount)
{
	if (!HasImage()) {
		return;
	}

	CommitImage(CreateFilteredImage(type, amount));
}

PixelImage ImageDocument::CreateFilteredImage(FilterType type, float amount) const
{
	PixelImage image = Current();

	ImageFilterFactory::Create(type, amount)->Apply(image);

	return image;
}

void ImageDocument::CommitImage(PixelImage image)
{
	Commit(std::move(image));
}

void ImageDocument::ResetToOriginal()
{
	if (HasImage() && m_historyIndex != 0) {
		m_historyIndex = 0;
	}
}

void ImageDocument::Undo()
{
	if (CanUndo()) {
		--m_historyIndex;
	} 
}

void ImageDocument::Redo()
{
	if (CanRedo()) {
		++m_historyIndex;
	}
}

bool ImageDocument::HasImage() const noexcept { return !m_history.empty(); }
bool ImageDocument::CanUndo() const noexcept { return HasImage() && m_historyIndex > 0; }
bool ImageDocument::CanRedo() const noexcept { return HasImage() && m_historyIndex + 1 < m_history.size(); }
const PixelImage& ImageDocument::Current() const noexcept { return m_history[m_historyIndex]; }
const std::wstring& ImageDocument::SourcePath() const noexcept { return m_sourcePath; }

IP_NAMESPACE_END
