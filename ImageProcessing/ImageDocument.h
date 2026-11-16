#pragma once

#include "Filter.h"

IP_NAMESPACE_START

struct PixelImage final
{
	int width{};
	int height{};
	std::vector<std::uint8_t> pixels;

	[[nodiscard]] bool IsValid() const noexcept;
};

class ImageDocument final
{
public:
	[[nodiscard]] bool Load(const std::wstring& path, std::wstring& error);
	[[nodiscard]] bool Save(const std::wstring& path, std::wstring& error) const;

	void ResetHistory();
	void ClearImage();
	void ResetToOriginal();
	void Undo();
	void Redo();

	void ApplyFilter(FilterType type, float amount = 30.0f);
	void CommitImage(PixelImage image);

	[[nodiscard]] PixelImage CreateFilteredImage(FilterType type, float amount = 30.0f) const;

	[[nodiscard]] bool HasImage() const noexcept;
	[[nodiscard]] bool CanUndo() const noexcept;
	[[nodiscard]] bool CanRedo() const noexcept;

	[[nodiscard]] const PixelImage& Current() const noexcept;
	[[nodiscard]] const std::wstring& SourcePath() const noexcept;

private:
	void Commit(PixelImage image);

	std::vector<PixelImage> m_history;
	std::size_t m_historyIndex{};
	std::wstring m_sourcePath;
};

IP_NAMESPACE_END
