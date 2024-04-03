#pragma once
#include <iostream>
#include <qstring.h>

class IOToolBox
{
	static inline FILE* currentFile = nullptr;

public:
	static QString ReadFile(const QString& _path);
	static bool WriteInFile(const QString& _path, const QString& _toWrite, bool _flushFileContent = false);
	static QString GetFileName(const QString& _completePath);
	static bool CreateFile(const QString& _completePath,const QString& _toWrite = "");
	static std::vector<QString> GetAllLines(const QString& _fileContent);

#pragma region FolderMethods
	static bool IsFolderExisting(const QString& _folderName);
	static std::vector<QString> GetFilesInFolder(const QString& _folderPath);
	static bool CreateFolder(const QString& _folderPath);
#pragma endregion
private:
	static bool OpenFile(const QString& _path, bool _flushFileContent);
	static bool CloseFile();
	static int GetFileSize();
};

