#include "pch.h"
#include "Helpers.h"
#include "ImageDocument.h"

IP_NAMESPACE_START

GdiplusSession::GdiplusSession()
{
	Gdiplus::GdiplusStartupInput input;
	if (Gdiplus::GdiplusStartup(&m_token, &input, nullptr) != Gdiplus::Ok) {
		m_token = 0;
	}
}

GdiplusSession::~GdiplusSession()
{
	if (m_token != 0) {
		Gdiplus::GdiplusShutdown(m_token);
	}
}

bool GdiplusSession::IsInitialized() const noexcept
{
	return m_token != 0;
}

OpenGlTexture::OpenGlTexture()
{
	glGenTextures(1, &m_texture);
}

OpenGlTexture::~OpenGlTexture()
{
	if (m_texture != 0) {
		glDeleteTextures(1, &m_texture);
	}
}

OpenGlTexture::OpenGlTexture(OpenGlTexture&& other) noexcept
	: m_texture(std::exchange(other.m_texture, 0))
{
}

OpenGlTexture& OpenGlTexture::operator=(OpenGlTexture&& other) noexcept
{
	if (this != &other) {
		if (m_texture != 0) {
			glDeleteTextures(1, &m_texture);
		}
		m_texture = std::exchange(other.m_texture, 0);
	}
	return *this;
}

void OpenGlTexture::Upload(const PixelImage& image)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint OpenGlTexture::Get() const noexcept
{
	return m_texture;
}

BitmapLock::BitmapLock(Gdiplus::Bitmap& bitmap, const Gdiplus::Rect& bounds, UINT lockMode)
	: m_bitmap(&bitmap), m_locked(m_bitmap->LockBits(&bounds, lockMode, PixelFormat32bppARGB, &m_data) == Gdiplus::Ok ? 1 : 0)
{
}

BitmapLock::~BitmapLock()
{
	if (m_locked != 0) {
		m_bitmap->UnlockBits(&m_data);
	}
}

bool BitmapLock::IsLocked() const noexcept
{
	return m_locked != 0;
}

const Gdiplus::BitmapData& BitmapLock::Data() const noexcept
{
	return m_data;
}

Gdiplus::BitmapData& BitmapLock::Data() noexcept
{
	return m_data;
}

bool Helpers::OpenImageDialog(std::wstring& path)
{
	std::array<wchar_t, MAX_PATH> fileName{};

	OPENFILENAMEW dialog{};
	dialog.lStructSize = sizeof(dialog);
	dialog.lpstrFile = fileName.data();
	dialog.nMaxFile = static_cast<DWORD>(fileName.size());
	dialog.lpstrFilter = OPEN_DIALOG_FILTER.data();
	dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	if (GetOpenFileNameW(&dialog) == FALSE) {
		return false;
	}

	path = fileName.data();
	return true;
}

bool Helpers::SaveImageDialog(std::wstring& path)
{
	std::array<wchar_t, MAX_PATH> fileName{};
	wcsncpy_s(fileName.data(), fileName.size(), DEFAULT_SAVE_FILE_NAME.data(), _TRUNCATE);

	OPENFILENAMEW dialog{};
	dialog.lStructSize = sizeof(dialog);
	dialog.lpstrFile = fileName.data();
	dialog.nMaxFile = static_cast<DWORD>(fileName.size());
	dialog.lpstrFilter = SAVE_DIALOG_FILTER.data();
	dialog.lpstrDefExt = DEFAULT_SAVE_EXTENSION.data();
	dialog.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

	if (GetSaveFileNameW(&dialog) == FALSE) {
		return false;
	}

	path = fileName.data();
	return true;
}

void Helpers::ShowError(std::wstring_view message)
{
	MessageBoxW(nullptr, std::wstring(message).c_str(), WINDOW_TITLE_WIDE.data(), MB_OK | MB_ICONERROR);
}

std::uint8_t Helpers::ClampByte(int value) noexcept
{
	return static_cast<std::uint8_t>(std::clamp(value, 0, MAX_COLOR_VALUE));
}

void Helpers::SetHandCursorWhenHovered()
{
	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
}

std::wstring Helpers::GdiStatusMessage(Gdiplus::Status status)
{
	if (status == Gdiplus::InvalidParameter) {
		return std::wstring(INVALID_IMAGE_ERROR);
	}
	if (status == Gdiplus::OutOfMemory) {
		return std::wstring(OUT_OF_MEMORY_ERROR);
	}
	if (status == Gdiplus::FileNotFound) {
		return std::wstring(FILE_NOT_FOUND_ERROR);
	}
	return std::wstring(GENERIC_IMAGE_ERROR);
}

bool Helpers::GetEncoderClsid(std::wstring_view extension, CLSID& clsid)
{
	UINT count{};
	UINT size{};
	if (Gdiplus::GetImageEncodersSize(&count, &size) != Gdiplus::Ok || size == 0) {
		return false;
	}

	std::vector<std::uint8_t> buffer(size);
	Gdiplus::ImageCodecInfo* encoders = reinterpret_cast<Gdiplus::ImageCodecInfo*>(buffer.data());
	if (Gdiplus::GetImageEncoders(count, size, encoders) != Gdiplus::Ok) {
		return false;
	}

	std::wstring normalized(extension);
	for (wchar_t& character : normalized) {
		character = static_cast<wchar_t>(std::towlower(character));
	}

	const std::wstring mime = normalized == PNG_EXTENSION ? std::wstring(PNG_MIME_TYPE) :
		normalized == BMP_EXTENSION ? std::wstring(BMP_MIME_TYPE) :
		normalized == JPG_EXTENSION || normalized == JPEG_EXTENSION ? std::wstring(JPG_MIME_TYPE) :
		normalized == GIF_EXTENSION ? std::wstring(GIF_MIME_TYPE) : std::wstring(PNG_MIME_TYPE);

	for (UINT index = 0; index < count; ++index) {
		if (mime == encoders[index].MimeType) {
			clsid = encoders[index].Clsid;
			return true;
		}
	}
	return false;
}

std::wstring Helpers::FileExtension(std::wstring_view path)
{
	const std::size_t separator = path.find_last_of(PATH_SEPARATORS);
	const std::size_t dot = path.find_last_of(L'.');
	return dot != std::wstring::npos && (separator == std::wstring::npos || dot > separator)
		? std::wstring(path.substr(dot))
		: std::wstring(EMPTY_WIDE_STRING);
}

bool Helpers::ReadBitmapPixels(Gdiplus::Bitmap& bitmap, PixelImage& image, std::wstring& error)
{
	const Gdiplus::Rect bounds(0, 0, image.width, image.height);
	BitmapLock lock(bitmap, bounds, Gdiplus::ImageLockModeRead);
	if (!lock.IsLocked()) {
		error = PIXELS_READ_ERROR;
		return false;
	}

	const Gdiplus::BitmapData& data = lock.Data();
	for (int y = 0; y < image.height; ++y) {
		const auto* source = static_cast<const std::uint8_t*>(data.Scan0) + static_cast<std::ptrdiff_t>(y) * data.Stride;
		auto* destination = image.pixels.data() + static_cast<std::size_t>(y) * static_cast<std::size_t>(image.width) * PIXEL_CHANNEL_COUNT;
		if (!source || !destination) {
			error = PIXELS_PROCESS_ERROR;
			return false;
		}

		for (int x = 0; x < image.width; ++x) {
			destination[x * PIXEL_CHANNEL_COUNT + 0] = source[x * PIXEL_CHANNEL_COUNT + 2];
			destination[x * PIXEL_CHANNEL_COUNT + 1] = source[x * PIXEL_CHANNEL_COUNT + 1];
			destination[x * PIXEL_CHANNEL_COUNT + 2] = source[x * PIXEL_CHANNEL_COUNT + 0];
			destination[x * PIXEL_CHANNEL_COUNT + 3] = source[x * PIXEL_CHANNEL_COUNT + 3];
		}
	}
	return true;
}

bool Helpers::WriteBitmapPixels(Gdiplus::Bitmap& bitmap, const PixelImage& image, std::wstring& error)
{
	const Gdiplus::Rect bounds(0, 0, image.width, image.height);
	BitmapLock lock(bitmap, bounds, Gdiplus::ImageLockModeWrite);
	if (!lock.IsLocked()) {
		error = PIXELS_SAVE_ERROR;
		return false;
	}

	const Gdiplus::BitmapData& data = lock.Data();
	for (int y = 0; y < image.height; ++y) {
		uint8_t* destination = static_cast<std::uint8_t*>(data.Scan0) + static_cast<std::ptrdiff_t>(y) * data.Stride;
		const uint8_t* source = image.pixels.data() + static_cast<std::size_t>(y) * static_cast<std::size_t>(image.width) * PIXEL_CHANNEL_COUNT;
		if (!source || !destination) {
			error = PIXELS_SAVE_ERROR;
			return false;
		}

		for (int x = 0; x < image.width; ++x) {
			destination[x * PIXEL_CHANNEL_COUNT + 0] = source[x * PIXEL_CHANNEL_COUNT + 2];
			destination[x * PIXEL_CHANNEL_COUNT + 1] = source[x * PIXEL_CHANNEL_COUNT + 1];
			destination[x * PIXEL_CHANNEL_COUNT + 2] = source[x * PIXEL_CHANNEL_COUNT + 0];
			destination[x * PIXEL_CHANNEL_COUNT + 3] = source[x * PIXEL_CHANNEL_COUNT + 3];
		}
	}
	return true;
}

IP_NAMESPACE_END
