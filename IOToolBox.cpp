#include "IOToolBox.h"
#include <qfile.h>

QString IOToolBox::ReadFile(const QString& _path)
{
    QString _toRet;
    if (!OpenFile(_path, false) || !currentFile) return _toRet;
    const int _fileSize = GetFileSize();
    char* _buffer = new char[_fileSize + 1] {'\0'};
    const size_t _red = fread_s(_buffer, _fileSize, 1, _fileSize, currentFile);
    _buffer[_fileSize] = '\0';
    rewind(currentFile);
    _toRet = QString(_buffer);
    CloseFile();
    delete[] _buffer;
    return _toRet;
}

bool IOToolBox::WriteInFile(const QString& _path, const QString& _toWrite, bool _flushFileContent)
{
    if (!OpenFile(_path, _flushFileContent)) return false;
    int _error = fseek(currentFile, _flushFileContent ? 0 : SEEK_END, 0);
    if (_error != 0) return false;
    _error = fwrite(_toWrite.toStdString().c_str(), 1, _toWrite.length(), currentFile);
    CloseFile();
    return true;
}

QString IOToolBox::GetFileName(const QString& _completePath)
{
    QString _toRet = _completePath.last(_completePath.lastIndexOf("\\")+1);
    return _toRet;
}

bool IOToolBox::CreateFile(const QString& _completePath, const QString& _toWrite)
{
    QString _folder = _completePath.first(_completePath.lastIndexOf("/"));
    if (!IsFolderExisting(_folder))
        CreateFolder(_folder);
    const errno_t _error = fopen_s(&currentFile, _completePath.toStdString().c_str(), "w");
    if (_error != 0) return false;
    fwrite(_toWrite.toStdString().c_str(), sizeof(char), _toWrite.count(), currentFile);
    CloseFile();
    return true;
}

std::vector<QString> IOToolBox::GetAllLines(const QString& _fileContent)
{
    QString _toUse = "";
    std::vector<QString> _lines = std::vector<QString>();
    const int _size = _fileContent.size();
    if (_fileContent.isEmpty()) return _lines;
    for (int _i = 0; _i <= _size; _i++)
    {
        if (_i == _size ||_fileContent[_i] == '\n' || _fileContent[_i] == '\0')
        {
            if (_toUse.isEmpty()) continue;
            _lines.insert(_lines.end(), _toUse);
            _toUse = "";
        }
        else
            _toUse += _fileContent[_i];
    }
    return _lines;
}

bool IOToolBox::FileExist(const QString& _path)
{
    return QFile::exists(_path);
}

bool IOToolBox::IsFolderExisting(const QString& _folderPath)
{
    return  std::filesystem::exists(_folderPath.toStdString());;
}

std::vector<QString> IOToolBox::GetFilesInFolder(const QString& _folderPath)
{
   std::vector<QString> _toRet = std::vector<QString>();
   if (!IsFolderExisting(_folderPath)) return _toRet;

   for (const std::filesystem::directory_entry& _entry : std::filesystem::directory_iterator(_folderPath.toStdString()))
   {
       if (_entry.is_directory())
           continue;
       QString _path = _entry.path().string().c_str();
       _path.replace("\\", "/");
       _toRet.push_back(_path);
   }
   return _toRet;
}

bool IOToolBox::CreateFolder(const QString& _folderPath)
{
    return std::filesystem::create_directory(_folderPath.toStdString());
}

bool IOToolBox::OpenFile(const QString& _path, bool _flushFileContent)
{
    const errno_t _error = fopen_s(&currentFile, _path.toStdString().c_str(), _flushFileContent ? "w+" : "r+");
    return _error == 0;
}

bool IOToolBox::CloseFile()
{
    const int _error = fclose(currentFile);
    return _error == 0;
}

int IOToolBox::GetFileSize()
{
    const int _error = fseek(currentFile, 0, SEEK_END);
    if (_error != 0) return 0;
    const int _toRet = ftell(currentFile);
    rewind(currentFile);
    return _toRet;
}