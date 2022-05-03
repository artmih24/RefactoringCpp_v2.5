#include "Formatter.h"

// Get the text to be formatted.
// Usually the text would be obtained from an edit control.
char* Formatter::GetText(const string& filePath)
{   // open input file
    ifstream in(filePath.c_str());
    if (!in)
        cerr << "Cannot open input file " + filePath << endl;

    // get length of buffer
    const int bufferSizeIn = 131072;     // 128 KB

    // allocate memory
    char* bufferIn = new (nothrow) char[bufferSizeIn];
    if (bufferIn == NULL)
    {
        in.close();
        cerr << "Memory allocation failure on input" << endl;
    }

    // read data as a block
    in.read(bufferIn, bufferSizeIn);
    // get actual size - must be smaller than buffer size
    int textSizeIn = static_cast<int>(in.gcount());
    if (textSizeIn > bufferSizeIn)
    {
        in.close();
        cerr << "Read buffer is too small" << endl;
    }
    bufferIn[textSizeIn] = '\0';
    in.close();

    return bufferIn;
}

// Return the formatted text.
// Usually the text would be returned to an edit control.
void Formatter::SetText(const char* textOut, const string& filePathStr)
{   // create a backup file
    const char* filePath = filePathStr.c_str();
    string origfilePathStr = filePathStr + ".orig";
    const char* origfilePath = origfilePathStr.c_str();
    remove(origfilePath);              // remove a pre-existing file

    if (rename(filePath, origfilePath) < 0)
        cerr << "Cannot open input file " + filePathStr << endl;

    // open the output file
    ofstream out(filePath);
    if (!out)
        cerr << "Cannot open output file " + filePathStr << endl;

    // write the text
    size_t textSizeOut = strlen(textOut);
    out.write(textOut, static_cast<streamsize>(textSizeOut));
    out.close();
}

void STDCALL Formatter::ASErrorHandler(int errorNumber, const char* errorMessage)
{
    cout << "astyle error " << errorNumber << "\n" << errorMessage << endl;
}

char* STDCALL Formatter::ASMemoryAlloc(unsigned long memoryNeeded)
{
    char* buffer = new (nothrow) char[memoryNeeded];
    return buffer;
}

char* Formatter::FormatText(char* textIn)
{
    const char* options = "-A2tOP";
    return AStyleMain(textIn, options, ASErrorHandler, ASMemoryAlloc);
}
