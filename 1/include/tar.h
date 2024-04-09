// -------------- Архивация ---------------

// tar() - функция для архивации.
// - target_path - это путь к папке, которую надо заархивировать;
// - tar_path - это путь к папке, в которую следует сохранить архив.
// Функция возвращает 0 в случае успешной архивации, 1 при ошибках.
int tar(const char *target_path, const char *tar_path);

int packFolder(const char *folder_path, const char *tar_path, int depth, int header_descriptor, int *count);

int packFile(const char *sourceFilePath, const char *destinationFilePath, int descriptorDestinationFile, int depth, int header_descriptor, int *count);

// -------------- Разархивация ---------------

// untar() - функция для разархивации.
// - target_path - это путь к архиву, который надо разархивировать;
// - untar_path - это путь к папке, в которую следует разархировать архив.
// Функция возвращает 0 в случае успешной разархивации, 1 при ошибках.
int untar(const char *target_path, const char *untar_path);

int UnpackFolder(const char *folder_path, const char *tar_path, int depth);

int UnpackFile(const char *file_name, int file_size, int source_descriptor);