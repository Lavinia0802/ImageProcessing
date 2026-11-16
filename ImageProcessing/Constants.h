#pragma once

IP_NAMESPACE_START

inline constexpr int WINDOW_WIDTH = 1280;
inline constexpr int WINDOW_HEIGHT = 720;

inline constexpr std::string_view WINDOW_TITLE = "ImageProcessing";
inline constexpr std::wstring_view WINDOW_TITLE_WIDE = L"ImageProcessing";

inline constexpr std::string_view GLSL_VERSION = "#version 130";

inline constexpr std::string_view EDITOR_WINDOW_TITLE = "Image Editor";
inline constexpr std::string_view IMAGE_WORKSPACE_ID = "ImageWorkspace";
inline constexpr std::string_view FILTER_PANEL_ID = "FilterPanel";
inline constexpr std::string_view OPEN_IMAGE_LABEL = "Open Image";
inline constexpr std::string_view SAVE_IMAGE_LABEL = "Save Image";
inline constexpr std::string_view UNDO_LABEL = "Undo";
inline constexpr std::string_view REDO_LABEL = "Redo";
inline constexpr std::string_view EMPTY_IMAGE_LABEL = "Please select an image.";
inline constexpr std::string_view FILTERS_LABEL = "Filters";
inline constexpr std::string_view GRAYSCALE_LABEL = "Grayscale";
inline constexpr std::string_view INVERT_LABEL = "Invert";
inline constexpr std::string_view SEPIA_LABEL = "Sepia";
inline constexpr std::string_view BRIGHTEN_LABEL = "Brighten";
inline constexpr std::string_view RESET_LABEL = "Reset";
inline constexpr std::string_view FILTER_HISTORY_STATUS = "Undo/redo tracks every filter.";
inline constexpr std::string_view NO_IMAGE_STATUS = "Load an image to enable filters.";
inline constexpr std::string_view INITIAL_STATUS = "Select an image to begin.";
inline constexpr std::string_view IMAGE_LOADED_STATUS = "Image loaded.";
inline constexpr std::string_view IMAGE_SAVED_STATUS = "Image saved.";
inline constexpr std::string_view FILTER_PROCESSING_STATUS = "Processing filter...";
inline constexpr std::string_view FILTER_APPLIED_STATUS = "Filter applied.";
inline constexpr std::string_view IMAGE_REMOVED_STATUS = "Image removed.";
inline constexpr std::string_view NULL_WINDOW_ERROR = "Renderer: GLFW window pointer is null.";
inline constexpr std::string_view UNKNOWN_FILTER_ERROR = "Unknown image filter type.";
inline constexpr std::string_view IMGUI_IO_ERROR = "ImGuiContext: Failed to retrieve ImGuiIO instance. Ensure that the ImGui context is properly initialized.";
inline constexpr std::string_view GLFW_INIT_ERROR = "Failed to initialize GLFW\n";
inline constexpr std::string_view GLFW_WINDOW_ERROR = "Failed to create GLFW window\n";

inline constexpr float LEFT_PANEL_RATIO = 0.7f;
inline constexpr float MIN_IMAGE_SCALE = 0.05f;
inline constexpr float MAX_IMAGE_SCALE = 1.0f;
inline constexpr float EMPTY_IMAGE_CURSOR_OFFSET_X = 85.0f;
inline constexpr float EMPTY_IMAGE_CURSOR_OFFSET_Y = 20.0f;
inline constexpr float EMPTY_IMAGE_SELECTABLE_WIDTH = 170.0f;
inline constexpr float FILTER_BRIGHTNESS_AMOUNT = 30.0f;
inline constexpr std::array<float, 3> APP_BACKGROUND_COLOR{ 0.07f, 0.08f, 0.11f };
inline constexpr float OPENGL_CLEAR_ALPHA = 1.0f;

inline constexpr std::wstring_view OPEN_DIALOG_FILTER = L"Image files (*.png;*.jpg;*.jpeg;*.bmp;*.gif)\0*.png;*.jpg;*.jpeg;*.bmp;*.gif\0All files (*.*)\0*.*\0";
inline constexpr std::wstring_view SAVE_DIALOG_FILTER = L"PNG image (*.png)\0*.png\0JPEG image (*.jpg)\0*.jpg\0Bitmap image (*.bmp)\0*.bmp\0";
inline constexpr std::wstring_view DEFAULT_SAVE_FILE_NAME = L"filtered-image.png";
inline constexpr std::wstring_view DEFAULT_SAVE_EXTENSION = L"png";
inline constexpr std::wstring_view PNG_EXTENSION = L".png";
inline constexpr std::wstring_view BMP_EXTENSION = L".bmp";
inline constexpr std::wstring_view JPG_EXTENSION = L".jpg";
inline constexpr std::wstring_view JPEG_EXTENSION = L".jpeg";
inline constexpr std::wstring_view GIF_EXTENSION = L".gif";
inline constexpr std::wstring_view PNG_MIME_TYPE = L"image/png";
inline constexpr std::wstring_view BMP_MIME_TYPE = L"image/bmp";
inline constexpr std::wstring_view JPG_MIME_TYPE = L"image/jpeg";
inline constexpr std::wstring_view GIF_MIME_TYPE = L"image/gif";
inline constexpr std::wstring_view PATH_SEPARATORS = L"\\/";
inline constexpr std::wstring_view EMPTY_WIDE_STRING = L"";

inline constexpr std::wstring_view INVALID_IMAGE_ERROR = L"The image is invalid or uses an unsupported format.";
inline constexpr std::wstring_view OUT_OF_MEMORY_ERROR = L"There was not enough memory to process the image.";
inline constexpr std::wstring_view FILE_NOT_FOUND_ERROR = L"The image file could not be found.";
inline constexpr std::wstring_view GENERIC_IMAGE_ERROR = L"The image operation failed.";
inline constexpr std::wstring_view PIXELS_READ_ERROR = L"The image pixels could not be read.";
inline constexpr std::wstring_view PIXELS_PROCESS_ERROR = L"The image pixels could not be prepared for processing.";
inline constexpr std::wstring_view NO_IMAGE_SAVE_ERROR = L"Select an image before saving.";
inline constexpr std::wstring_view PIXELS_SAVE_ERROR = L"The image pixels could not be prepared for saving.";
inline constexpr std::wstring_view UNSUPPORTED_OUTPUT_ERROR = L"The selected output format is not supported.";

inline constexpr int MAX_COLOR_VALUE = 255;
inline constexpr std::size_t PIXEL_CHANNEL_COUNT = 4U;
inline constexpr float GRAYSCALE_RED_WEIGHT = 0.299f;
inline constexpr float GRAYSCALE_GREEN_WEIGHT = 0.587f;
inline constexpr float GRAYSCALE_BLUE_WEIGHT = 0.114f;
inline constexpr float SEPIA_RED_RED_WEIGHT = 0.393f;
inline constexpr float SEPIA_RED_GREEN_WEIGHT = 0.769f;
inline constexpr float SEPIA_RED_BLUE_WEIGHT = 0.189f;
inline constexpr float SEPIA_GREEN_RED_WEIGHT = 0.349f;
inline constexpr float SEPIA_GREEN_GREEN_WEIGHT = 0.686f;
inline constexpr float SEPIA_GREEN_BLUE_WEIGHT = 0.168f;
inline constexpr float SEPIA_BLUE_RED_WEIGHT = 0.272f;
inline constexpr float SEPIA_BLUE_GREEN_WEIGHT = 0.534f;
inline constexpr float SEPIA_BLUE_BLUE_WEIGHT = 0.131f;

IP_NAMESPACE_END
