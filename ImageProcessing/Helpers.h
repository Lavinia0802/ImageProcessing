#pragma once

IP_NAMESPACE_START

struct PixelImage;

class GdiplusSession final
{
public:
	GdiplusSession();
	~GdiplusSession();

	[[nodiscard]] bool IsInitialized() const noexcept;

private:
	GdiplusSession(const GdiplusSession&) = delete;
	GdiplusSession& operator=(const GdiplusSession&) = delete;
	GdiplusSession(GdiplusSession&&) = delete;
	GdiplusSession& operator=(GdiplusSession&&) = delete;

private:
	ULONG_PTR m_token{};
};

class OpenGlTexture final
{
public:
	OpenGlTexture();
	~OpenGlTexture();

	OpenGlTexture(OpenGlTexture&& other) noexcept;
	OpenGlTexture& operator=(OpenGlTexture&& other) noexcept;

	void Upload(const PixelImage& image);
	[[nodiscard]] GLuint Get() const noexcept;

private:
	OpenGlTexture(const OpenGlTexture&) = delete;
	OpenGlTexture& operator=(const OpenGlTexture&) = delete;

private:
	GLuint m_texture{};
	std::uint32_t m_reserved{}; // reserved for future use, currently unused - additional padding to avoid compiler warning
};

class BitmapLock final
{
public:
	BitmapLock(Gdiplus::Bitmap& bitmap, const Gdiplus::Rect& bounds, UINT lockMode);
	~BitmapLock();

	BitmapLock(const BitmapLock&) = delete;
	BitmapLock& operator=(const BitmapLock&) = delete;
	BitmapLock(BitmapLock&&) = delete;
	BitmapLock& operator=(BitmapLock&&) = delete;

	[[nodiscard]] bool IsLocked() const noexcept;
	[[nodiscard]] const Gdiplus::BitmapData& Data() const noexcept;
	[[nodiscard]] Gdiplus::BitmapData& Data() noexcept;

private:
	Gdiplus::Bitmap* m_bitmap{};
	Gdiplus::BitmapData m_data{};
	ULONG_PTR m_locked{};
};

class Helpers final
{
public:
	[[nodiscard]] static bool OpenImageDialog(std::wstring& path);
	[[nodiscard]] static bool SaveImageDialog(std::wstring& path);

	[[nodiscard]] static std::uint8_t ClampByte(int value) noexcept;

	static void ShowError(std::wstring_view message);
	static void SetHandCursorWhenHovered();

	[[nodiscard]] static bool GetEncoderClsid(std::wstring_view extension, CLSID& clsid);
	[[nodiscard]] static std::wstring GdiStatusMessage(Gdiplus::Status status);
	[[nodiscard]] static std::wstring FileExtension(std::wstring_view path);
	[[nodiscard]] static bool ReadBitmapPixels(Gdiplus::Bitmap& bitmap, PixelImage& image, std::wstring& error);
	[[nodiscard]] static bool WriteBitmapPixels(Gdiplus::Bitmap& bitmap, const PixelImage& image, std::wstring& error);
};

IP_NAMESPACE_END
