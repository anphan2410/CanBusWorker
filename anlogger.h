#ifndef ANLOGGER_H
#define ANLOGGER_H
//PREREQUISITE: -std=c++11
//NOTICE: On Windows, Only If _anLoggerSafeModeForWindowsEnabled Is Set,
//              Then An Logger Is Guaranteed To Be Thread-Safe
/************* Control Flags ****************************************************/
//_anLoggerEnabled Is Zero => Logger Is Globally Disabled
#define _anLoggerEnabled 1
//_anLoggerEnabled Is Zero => Logger Message Is Not Verbosely Positioned
#define _anPositionEnabled 1
/************* Performance Flags ************************************************/
//_anLoggerSafeModeForWindowsEnabled is only used for windows
//If _anLoggerSafeModeForWindowsEnabled Is Set,
//Then Logger Message Text Attribute Is Disabled
//In Return For Thread-Safe Operation
#define _anLoggerSafeModeForWindowsEnabled 0
//_anLoggerVividModeForLinuxEnabled is only used for linux
//If _anLoggerVividModeForLinuxEnabled Is Set,
//Then Logger Message Text Becomes Bold And Brighter
#define _anLoggerVividModeForLinuxEnabled 0
#define _anMessagePathTextAttribute anDefaultTextAttribute
#define _anThreadIdPositionEnabled 1
#define _anFunctionPositionEnabled 1
#define _anFilePositionEnabled 1
#define _anLinePositionEnabled 1
#define _anTimePositionEnabled 1
/********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <chrono>
#include <time.h>
#include <vector>
#if defined _WINDOWS_ || defined __WINDOWS__\
        || defined _WIN16 || defined __TOS_WIN__\
        || defined _WIN32_WCE || defined _WIN32_WCE\
        || defined WIN32 || defined _WIN32 || defined _WIN32_\
        || defined __WIN32 || defined __WIN32__\
        || defined _WIN32_WINNT || defined WINNT || defined _WINNT\
        || defined _WINNT_ || defined __WINNT || defined __WINNT__
    #include <windows.h>
    #define __anWINOS__
#elif defined linux || defined _linux_ || defined __linux || defined __linux__\
        || defined __gnu_linux__
    #define __anLINUXOS__
#endif
#ifdef QT_CORE_LIB
    #include <QtGlobal>
    #include <QDebug>
    #include <QVariant>
    #include <QString>
    #include <QTime>
    #include <QTextStream>
    #include <QIODevice>
    #define __anQt__
#endif
/********************************************************************************/

static char anStdErrBuffer[BUFSIZ];

#define anTxtAttribType unsigned short

#ifdef __anWINOS__

/*  8Bit - ConsoleTextAttribute
    bit 0 - foreground blue
    bit 1 - foreground green
    bit 2 - foreground red
    bit 3 - foreground intensity
    bit 4 - background blue
    bit 5 - background green
    bit 6 - background red
    bit 7 - background intensity */
    #define anForegroundBlack 0
    #define anForegroundRed 0b00001100
    #define anForegroundGreen 0b00001010
    #define anForegroundYellow 0b00001110
    #define anForegroundBlue 0b00001001
    #define anForegroundMagenta 0b00001101
    #define anForegroundCyan 0b00001011
    #define anForegroundWhite 0b00001111
    #define anDefaultTextAttribute 0b00000111

    inline static bool anGetCurrentConsoleTextAttribute(anTxtAttribType &OutputVar) {
        CONSOLE_SCREEN_BUFFER_INFO tmp;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &tmp))
        {
            OutputVar = tmp.wAttributes;
            return true;
        }
        return false;
    }
    #define _anGetConsoleTextAttribute(destination)\
        anGetCurrentConsoleTextAttribute(destination)

    #define anSetConsoleTextAttribute(TextAtrribute)\
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TextAtrribute)

    #define __anFilePathSlashChar__ u'\\'

#elif defined __anLINUXOS__

    #define anForegroundBlack 30
    #define anForegroundRed 31
    #define anForegroundGreen 32
    #define anForegroundYellow 33
    #define anForegroundBlue 34
    #define anForegroundMagenta 35
    #define anForegroundCyan 36
    #define anForegroundWhite 37
    #define anDefaultTextAttribute 0

    inline static bool anGetCurrentConsoleTextAttribute(anTxtAttribType &OutputVar) {
        std::string tmpBuff = std::string(anStdErrBuffer);
        std::size_t tmp = tmpBuff.find_last_of(u8"\033[");
        if (tmp != std::string::npos)
        {
            ++tmp;
            if ((tmpBuff.at(tmp) == u'0') && (tmpBuff.at(tmp+1) == u'm'))
                OutputVar = 0;
            else
            {
                if (tmpBuff.at(tmp) == u'1')
                    tmp += 2;
                if ((tmpBuff.at(tmp) == u'3') && (tmpBuff.at(tmp+2) == u'm'))
                    OutputVar = std::stoi(tmpBuff.substr(tmp,2));
                else
                    return false;
            }
            return true;
        }
        else
            return false;
    }
    #define _anGetConsoleTextAttribute(destination)\
        anGetCurrentConsoleTextAttribute(destination)

    inline static const std::string anSetConsoleTextAttributePrefixString(anTxtAttribType TxtAttrib) {
        if (TxtAttrib)
        {
            std::string tmp = u8"\033[";
            #if _anLoggerVividModeForLinuxEnabled
                tmp += u8"1;";
            #endif
            tmp += std::to_string(TxtAttrib);
            tmp += u8"m";
            return tmp;
        }
        else
            return u8"\033[0m";
    }

    #define anSetConsoleTextAttribute(TxtAttrib) \
        fprintf(stderr, u8"%s", anSetConsoleTextAttributePrefixString(TxtAttrib).c_str())

    #define __anFilePathSlashChar__ u'/'

#endif

#if _anPositionEnabled && (_anThreadIdPositionEnabled\
        || _anFunctionPositionEnabled || _anFilePositionEnabled\
        || _anLinePositionEnabled || _anTimePositionEnabled)

    #if _anThreadIdPositionEnabled
        #define __anPThreadId__ static_cast<unsigned long int>(pthread_self())
    #endif

    #if _anFilePositionEnabled
        inline static const std::string anGetCurrentFileName(const std::string &currentFilePath) {
            return currentFilePath.substr(
                        currentFilePath.find_last_of(__anFilePathSlashChar__)+1);
        }
        #define __anFILENAME__ anGetCurrentFileName(__FILE__)
    #endif

    #if _anTimePositionEnabled
        #define __anElapsedTimeNSEC__\
            std::chrono::duration_cast<std::chrono::nanoseconds>(\
                std::chrono::steady_clock::now() - __anStartTimePoint__).count()
    #endif

    inline static const std::string anCurrentMessagePath(
            #if _anTimePositionEnabled
                #if _anFunctionPositionEnabled || _anFilePositionEnabled\
                    || _anLinePositionEnabled || _anThreadIdPositionEnabled
                    const unsigned long int &currentElapsedTime,
                #else
                    const unsigned long int &currentElapsedTime
                #endif
            #endif
            #if _anThreadIdPositionEnabled
                #if _anFunctionPositionEnabled || _anFilePositionEnabled\
                    || _anLinePositionEnabled
                    const unsigned long int &currentThreadId,
                #else
                    const unsigned long int &currentThreadId
                #endif
            #endif
            #if _anFunctionPositionEnabled
                #if _anFilePositionEnabled || _anLinePositionEnabled
                    const std::string &currentFunctionName,
                #else
                    const std::string &currentFunctionName
                #endif

            #endif
            #if _anFilePositionEnabled
                #if _anLinePositionEnabled
                    const std::string &currentFileName,
                #else
                    const std::string &currentFileName
                #endif
            #endif
            #if _anLinePositionEnabled
                const unsigned int &currentLine
            #endif
            ) {
        std::string tmp = u8"";
            #if _anThreadIdPositionEnabled
                tmp += u8"|";
                tmp += std::to_string(currentThreadId);
            #endif
            #if _anFunctionPositionEnabled
                tmp += u8"|";
                tmp += currentFunctionName;
            #endif
            #if _anFilePositionEnabled
                tmp += u8"|";
                tmp += currentFileName;
            #endif
            #if _anLinePositionEnabled
                tmp += u8"|";
                tmp += std::to_string(currentLine);
            #endif
            #if _anTimePositionEnabled
                tmp += u8"|";
                tmp += std::to_string(currentElapsedTime);
            #endif
        tmp += u8"|";
        return tmp;
    }

    #if _anTimePositionEnabled
        #if _anFunctionPositionEnabled || _anFilePositionEnabled\
            || _anLinePositionEnabled || _anThreadIdPositionEnabled
            #define anTmpTimeParamForMsgPathMacro __anElapsedTimeNSEC__,
        #else
            #define anTmpTimeParamForMsgPathMacro __anElapsedTimeNSEC__
        #endif
    #else
        #define anTmpTimeParamForMsgPathMacro
    #endif

    #if _anThreadIdPositionEnabled
        #if _anFunctionPositionEnabled || _anFilePositionEnabled || _anLinePositionEnabled
            #define anTmpThreadIdParamForMsgPathMacro __anPThreadId__,
        #else
            #define anTmpThreadIdParamForMsgPathMacro __anPThreadId__
        #endif
    #else
        #define anTmpThreadIdParamForMsgPathMacro
    #endif

    #if _anFunctionPositionEnabled
        #if _anFilePositionEnabled || _anLinePositionEnabled
            #define anTmpFunctionParamForMsgPathMacro __PRETTY_FUNCTION__,
        #else
            #define anTmpFunctionParamForMsgPathMacro __PRETTY_FUNCTION__
        #endif
    #else
        #define anTmpFunctionParamForMsgPathMacro
    #endif

    #if _anFilePositionEnabled
        #if _anLinePositionEnabled
            #define anTmpFileNameParamForMsgPathMacro __anFILENAME__,
        #else
            #define anTmpFileNameParamForMsgPathMacro __anFILENAME__
        #endif
    #else
        #define anTmpFileNameParamForMsgPathMacro
    #endif

    #if _anLinePositionEnabled
        #define anTmpLineParamForMsgPathMacro __LINE__
    #else
        #define anTmpLineParamForMsgPathMacro
    #endif

    #define __anMessagePath__\
        anCurrentMessagePath(anTmpTimeParamForMsgPathMacro\
                             anTmpThreadIdParamForMsgPathMacro\
                             anTmpFunctionParamForMsgPathMacro\
                             anTmpFileNameParamForMsgPathMacro\
                             anTmpLineParamForMsgPathMacro)

#else
    #define _anPositionDisabled
#endif

/********************************************************************************/
#if _anLoggerEnabled
/********************************************************************************/

#ifdef __anQt__
    #define anTmpSaveMsgInputToAStdStrVar(msgIn,newMsgStrVar)\
            std::string newMsgStrVar;\
            {\
                QString * tmpQStrrBefore##newMsgStrVar = new QString(u8"");\
                QTextStream(tmpQStrrBefore##newMsgStrVar, QIODevice::ReadWrite) << msgIn;\
                newMsgStrVar = tmpQStrrBefore##newMsgStrVar->toStdString();\
                delete tmpQStrrBefore##newMsgStrVar;\
                tmpQStrrBefore##newMsgStrVar = Q_NULLPTR;\
            }
#else
    #define anTmpSaveMsgInputToAStdStrVar(msgIn,newMsgStrVar)\
            std::string newMsgStrVar;\
            {\
                std::stringstream tmpStrStrmBefore##newMsgStrVar;\
                tmpStrStrmBefore##newMsgStrVar << msgIn;\
                newMsgStrVar = tmpStrStrmBefore##newMsgStrVar.str();\
            }
#endif

#if defined __anWINOS__ && !_anLoggerSafeModeForWindowsEnabled
    #define anTmpOutputMsgStrVarDeclaration
    #define anTmpOutputMsgStrVarToStdErr
#else
    #define anTmpOutputMsgStrVar tmpOutputMsgStr
    #define anTmpOutputMsgStrVarDeclaration\
        std::string anTmpOutputMsgStrVar
    #define anTmpOutputMsgStrVarToStdErr\
        fprintf(stderr, u8"%s", anTmpOutputMsgStrVar.c_str())
#endif

#if defined __anWINOS__ && _anLoggerSafeModeForWindowsEnabled
    #define anTmpSaveTextAttributeToATxtAtribVar
#else
    #define anTmpPrevTxtAtribVar tmpPrevTxtAtribVar
    #define anTmpSaveTextAttributeToATxtAtribVar\
        anTxtAttribType anTmpPrevTxtAtribVar = __anOriginalConsoleTextAttribute__;\
        _anGetConsoleTextAttribute(anTmpPrevTxtAtribVar)
#endif

#ifdef _anPositionDisabled
    #define anTmpSaveCurrentMessagePathToAMsgPathStrVar
#else
    #define anTmpCurrentMessagePathStrVar aNowMsgPathStrVar
    #define anTmpSaveCurrentMessagePathToAMsgPathStrVar\
        std::string anTmpCurrentMessagePathStrVar = __anMessagePath__
#endif

inline static void anTmpNoLineMessageLogger(
                                    #if defined anTmpCurrentMessagePathStrVar\
                                            || defined anTmpOutputMsgStrVar
                                          const std::string &aNoLineMessage,
                                    #else
                                          const std::string &aNoLineMessage
                                    #endif
                                    #ifdef anTmpOutputMsgStrVar
                                        #if defined anTmpCurrentMessagePathStrVar
                                            std::string &destMsgStr,
                                        #else
                                            std::string &destMsgStr
                                        #endif
                                    #endif
                                    #ifdef anTmpCurrentMessagePathStrVar
                                        #ifdef anTmpPrevTxtAtribVar
                                            const std::string &msgPath,
                                            const anTxtAttribType &prePathAttrib
                                        #else
                                            const std::string &msgPath
                                        #endif
                                    #endif
                                            ) {
    #ifdef anTmpOutputMsgStrVar
        destMsgStr += aNoLineMessage;
        #ifdef anTmpCurrentMessagePathStrVar
            #ifdef anTmpPrevTxtAtribVar
                destMsgStr += anSetConsoleTextAttributePrefixString(_anMessagePathTextAttribute);
            #endif
            destMsgStr += msgPath;
            #ifdef anTmpPrevTxtAtribVar
                destMsgStr += anSetConsoleTextAttributePrefixString(prePathAttrib);
            #endif
        #endif
    #else
        fprintf(stderr, u8"%s", aNoLineMessage.c_str());
        std::cerr.flush();
        #ifdef anTmpCurrentMessagePathStrVar
            #ifdef anTmpPrevTxtAtribVar
                anSetConsoleTextAttribute(_anMessagePathTextAttribute);
            #endif
            fprintf(stderr, u8"%s", msgPath.c_str());
            std::cerr.flush();
            #ifdef anTmpPrevTxtAtribVar
                anSetConsoleTextAttribute(prePathAttrib);
            #endif
        #endif
    #endif
}

#if defined anTmpCurrentMessagePathStrVar\
        || defined anTmpOutputMsgStrVar
    #define anTmpNoLineMsgParamForNoLineMacro(noLineMsgVar) noLineMsgVar,
#else
    #define anTmpNoLineMsgParamForNoLineMacro(noLineMsgVar) noLineMsgVar
#endif

#ifdef anTmpOutputMsgStrVar
    #if defined anTmpCurrentMessagePathStrVar
        #define anTmpOutputMsgStrParamForNoLineMacro(destStr) destStr,
    #else
        #define anTmpOutputMsgStrParamForNoLineMacro(destStr) destStr
    #endif
#else
    #define anTmpOutputMsgStrParamForNoLineMacro(destStr)
#endif

#ifdef anTmpCurrentMessagePathStrVar
    #ifdef anTmpPrevTxtAtribVar
        #define anTmpMsgPathParamForNoLineMacro(pathStr) pathStr,
        #define anTmpPrevTxtAtribVarParamForNoLineMacro(prePathTxtAttrib) prePathTxtAttrib
    #else
        #define anTmpMsgPathParamForNoLineMacro(pathStr) pathStr
        #define anTmpPrevTxtAtribVarParamForNoLineMacro(prePathTxtAttrib)
    #endif
#else
    #define anTmpMsgPathParamForNoLineMacro(pathStr)
    #define anTmpPrevTxtAtribVarParamForNoLineMacro(prePathTxtAttrib)
#endif

#define anTmpNoLineMessageLoggerMacro(noLineMsgVar, outStr, msgPath, preTxtAtt)\
    anTmpNoLineMessageLogger(anTmpNoLineMsgParamForNoLineMacro(noLineMsgVar)\
                             anTmpOutputMsgStrParamForNoLineMacro(outStr)\
                             anTmpMsgPathParamForNoLineMacro(msgPath)\
                             anTmpPrevTxtAtribVarParamForNoLineMacro(preTxtAtt))

inline static void anTmpMessageLogger(
                                #if defined anTmpPrevTxtAtribVar\
                                        || defined anTmpCurrentMessagePathStrVar\
                                        || defined anTmpOutputMsgStrVar
                                      std::string &rawMsgStr,
                                #else
                                      std::string &rawMsgStr
                                #endif
                                #ifdef anTmpOutputMsgStrVar
                                    #if defined anTmpPrevTxtAtribVar\
                                          || defined anTmpCurrentMessagePathStrVar
                                      std::string &destMsgStr,
                                    #else
                                      std::string &destMsgStr
                                    #endif
                                #endif
                                #ifdef anTmpPrevTxtAtribVar
                                      const anTxtAttribType &txtAttrib,
                                    #ifdef anTmpCurrentMessagePathStrVar
                                      const anTxtAttribType &previousTxtAttrib,
                                    #else
                                      const anTxtAttribType &previousTxtAttrib
                                    #endif
                                #endif
                                #ifdef anTmpCurrentMessagePathStrVar
                                      const std::string &currentMsgPath
                                #endif
                                      ) {
    #ifdef anTmpOutputMsgStrVar
        #ifdef anTmpPrevTxtAtribVar
            destMsgStr += anSetConsoleTextAttributePrefixString(txtAttrib);
        #endif
    #else
        #ifdef anTmpPrevTxtAtribVar
            anSetConsoleTextAttribute(txtAttrib);
        #endif
    #endif
    #ifdef anTmpCurrentMessagePathStrVar
        std::string nowPath = u8" <-";
        nowPath += currentMsgPath;
        std::string tmpPath = nowPath;
        tmpPath += u8"\n";
        #define anTmpRawMsgNoLineSubStrParamForNoLineMsgLoggerFunction rawMsgStr.substr(0,i++)
    #else
        #define anTmpRawMsgNoLineSubStrParamForNoLineMsgLoggerFunction rawMsgStr.substr(0,i++) + u8"\n"
    #endif
    for(std::string::size_type i = 0;(i = rawMsgStr.find(u8"\n", 0)) != std::string::npos;)
    {
        anTmpNoLineMessageLoggerMacro(anTmpRawMsgNoLineSubStrParamForNoLineMsgLoggerFunction, destMsgStr, tmpPath, txtAttrib);
        rawMsgStr = rawMsgStr.substr(i);
    }
    if (!rawMsgStr.empty())
        anTmpNoLineMessageLoggerMacro(rawMsgStr, destMsgStr, nowPath, txtAttrib);
    #ifdef anTmpOutputMsgStrVar
        #ifdef anTmpPrevTxtAtribVar
            destMsgStr += anSetConsoleTextAttributePrefixString(previousTxtAttrib);
        #endif
    #else
        #ifdef anTmpPrevTxtAtribVar
            anSetConsoleTextAttribute(previousTxtAttrib);
        #endif
    #endif
}

#if defined anTmpPrevTxtAtribVar\
        || defined anTmpCurrentMessagePathStrVar\
        || defined anTmpOutputMsgStrVar
    #define anTmpSrcMsgStrParamForMsgLoggerMacro(rawMsgStr) rawMsgStr,
#else
    #define anTmpSrcMsgStrParamForMsgLoggerMacro(rawMsgStr) rawMsgStr
#endif

#ifdef anTmpOutputMsgStrVar
    #if defined anTmpPrevTxtAtribVar\
          || defined anTmpCurrentMessagePathStrVar
        #define anTmpOutputMsgStrVarParamForMsgLoggerMacro(dstStr) dstStr,
    #else
        #define anTmpOutputMsgStrVarParamForMsgLoggerMacro(dstStr) dstStr
    #endif
#else
    #define anTmpOutputMsgStrVarParamForMsgLoggerMacro(dstStr)
#endif

#ifdef anTmpPrevTxtAtribVar
        #define anTmpTxtAtribVarParamForMsgLoggerMacro(txtAtt) txtAtt,
    #ifdef anTmpCurrentMessagePathStrVar
        #define anTmpPrevTxtAtribVarParamForMsgLoggerMacro(preTxtAtt) preTxtAtt,
    #else
        #define anTmpPrevTxtAtribVarParamForMsgLoggerMacro(preTxtAtt) preTxtAtt
    #endif
#else
    #define anTmpTxtAtribVarParamForMsgLoggerMacro(txtAtt)
    #define anTmpPrevTxtAtribVarParamForMsgLoggerMacro(preTxtAtt)
#endif

#ifdef anTmpCurrentMessagePathStrVar
    #define anTmpMsgPathParamForMsgLoggerMacro(aPathStr) aPathStr
#else
    #define anTmpMsgPathParamForMsgLoggerMacro(aPathStr)
#endif

#define anTmpMessageLoggerMacro(srcMsgStr, dstMsgStr, textAtt, prevTextAtt, pathStr)\
    anTmpMessageLogger(anTmpSrcMsgStrParamForMsgLoggerMacro(srcMsgStr)\
                       anTmpOutputMsgStrVarParamForMsgLoggerMacro(dstMsgStr)\
                       anTmpTxtAtribVarParamForMsgLoggerMacro(textAtt)\
                       anTmpPrevTxtAtribVarParamForMsgLoggerMacro(prevTextAtt)\
                       anTmpMsgPathParamForMsgLoggerMacro(pathStr))

/********************************************************************************/

    #define anLogCode(...) __VA_ARGS__

    #define anMsg(msg, txtAttrib) {\
            anTmpSaveTextAttributeToATxtAtribVar;\
            anTmpSaveCurrentMessagePathToAMsgPathStrVar;\
            anTmpSaveMsgInputToAStdStrVar(msg, tmpMsgInputStr);\
            anTmpOutputMsgStrVarDeclaration;\
            anTmpMessageLoggerMacro(tmpMsgInputStr,anTmpOutputMsgStrVar,txtAttrib,anTmpPrevTxtAtribVar,anTmpCurrentMessagePathStrVar);\
            anTmpOutputMsgStrVarToStdErr;\
            std::cerr.flush();}

    #define anTrk(msg) anMsg(msg << u8"\n", anForegroundWhite)

    #define anIf(condition,...) if (condition) {\
                                    __VA_ARGS__;}
    #define anInfo(msg) anMsg(u8"   " << msg << u8"\n", anForegroundWhite)
    #define anAck(msg) anMsg(u8"=> " << msg << u8"\n", anForegroundGreen)
    #define anWarn(msg) anMsg(u8"=> " << msg << u8"\n", anForegroundYellow)
    #define anError(msg) anMsg(u8"=> " << msg << u8"\n", anForegroundRed)

    #define anVar(var) anInfo(#var << u8" = " << var)

/********************************************************************************/

#else
    #define anLogCode(...)
    #define anMsg(msg)
    #define anIf(condition,...)
    #define anInfo(msg)
    #define anAck(msg)
    #define anWarn(msg)
    #define anError(msg)
    #define anVar(var)
    #define anTrk(msg)
#endif

/********************************************************************************/
static const anTxtAttribType anOriginalConsoleTextAttribute = [](){
    anTxtAttribType tmp = anDefaultTextAttribute;
    _anGetConsoleTextAttribute(tmp);
    return tmp;
}();
#define __anOriginalConsoleTextAttribute__ anOriginalConsoleTextAttribute
static const std::chrono::steady_clock::time_point anThisProgramStartingTimePoint = [](){
    setvbuf(stderr, anStdErrBuffer, _IOFBF, BUFSIZ);
    (void) anOriginalConsoleTextAttribute;
    return std::chrono::steady_clock::now();
}();
#define __anStartTimePoint__ anThisProgramStartingTimePoint
/********************************************************************************/
#endif // ANLOGGER_H
