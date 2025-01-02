#include "GUtils.h"
#include <QDateTime>

QString GUtils::unix2StrTimeQString(int64 time) {
    QDateTime dateTime = QDateTime::fromTime_t(time);
    QString formattedDateTime = dateTime.toString("yyyy-MM-dd HH:mm:ss");
    LOG_DBG("Formatted datetime: {}", formattedDateTime.toStdString());
    return formattedDateTime;
}

std::string GUtils::unix2StrTimeChrono(int64 time) {
    if (time == 0) {
        return std::string("");
    }
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);
    std::time_t localTime = std::chrono::system_clock::to_time_t(tp);
    std::tm* tmLocal = localtime(&localTime);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmLocal);
    std::string formattedDateTime(buffer);
    // LOG_DBG("Formatted datetime: {}", formattedDateTime);
    return formattedDateTime;
}

int64 GUtils::str2Unix(const std::string& str_time) {
    // 2025-01-02 22:22:00
    if (str_time.size() != 19 || str_time[4] != '-' || str_time[7] != '-' || str_time[10] != ' ' ||
        str_time[13] != ':' || str_time[16] != ':') {
        throw std::invalid_argument("Invalid date time format");
    }

    std::tm tm = {};
    tm.tm_year =
        (str_time[0] - '0') * 1000 + (str_time[1] - '0') * 100 + (str_time[2] - '0') * 10 + (str_time[3] - '0') - 1900;
    tm.tm_mon = (str_time[5] - '0') * 10 + (str_time[6] - '0') - 1;
    tm.tm_mday = (str_time[8] - '0') * 10 + (str_time[9] - '0');
    tm.tm_hour = (str_time[11] - '0') * 10 + (str_time[12] - '0');
    tm.tm_min = (str_time[14] - '0') * 10 + (str_time[15] - '0');
    tm.tm_sec = (str_time[17] - '0') * 10 + (str_time[18] - '0');

    if (mktime(&tm) == -1) {
        throw std::invalid_argument("Invalid date or time value");
    }

    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    return std::chrono::system_clock::to_time_t(tp);
}

std::string GUtils::getFilename(const std::string& path) {
    size_t pos = path.find_last_of("/\\");

    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    // ���û���ҵ�б�ܣ�˵�������ַ��������ļ���
    return path;
}

std::string GUtils::getFilenameWithoutExe(const std::string& path) {
    size_t namePos = path.find_last_of("/\\");

    std::string filename;
    if (namePos != std::string::npos) {
        filename = path.substr(namePos + 1);
    } else {
        filename = path;
    }

    size_t dosPos = filename.find_last_of(".");
    if (dosPos != std::string::npos && dosPos > 0) {
        return filename.substr(0, dosPos);
    }
    // ���û���ҵ���Ż��ߵ���ǵ�һ���ַ�������ԭʼ�ļ���
    return filename;
}

std::string GUtils::getFileSuffix(const std::string& path) {
    size_t namePos = path.find_last_of("/\\");

    std::string filename;
    if (namePos != std::string::npos) {
        filename = path.substr(namePos + 1);
    } else {
        filename = path;
    }

    size_t dosPos = filename.find_last_of(".");
    if (dosPos != std::string::npos && dosPos > 0) {
        return filename.substr(dosPos);
    }
    return "";
}

std::string GUtils::getAbsolutePath(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) {
        return path.substr(0, pos);
    }
    return path;
}

bool GUtils::isLetterC(char value) {
    if (value >= 'a' && value <= 'z') {
        return true;
    } else if (value >= 'A' && value <= 'Z') {
        return true;
    }
    return false;
}

bool GUtils::isLetterCplusplus(char value) {
    return std::isalpha(static_cast<unsigned char>(value)) != 0;
}

bool GUtils::splitStringAtDelimiter(const std::string& str, char delimiter, std::string& left, std::string& right) {
    size_t delimiterPos = str.find(delimiter);
    if (delimiterPos != std::string::npos) {
        // ��ȡ��߲��֣���ȥ��ǰ����β��ո�
        left = str.substr(0, delimiterPos);
        left.erase(0, left.find_first_not_of(" \t\n\r\f\v"));  // ȥ��ǰ���ո�
        left.erase(left.find_last_not_of(" \t\n\r\f\v") + 1);  // ȥ��β��ո�

        // ��ȡ�ұ߲��֣���ȥ��ǰ����β��ո�
        right = str.substr(delimiterPos + 1);
        right.erase(0, right.find_first_not_of(" \t\n\r\f\v"));  // ȥ��ǰ���ո�
        right.erase(right.find_last_not_of(" \t\n\r\f\v") + 1);  // ȥ��β��ո�
        return true;
    } else {
        // ���û���ҵ��ָ�������ԭ�ַ���Ϊ�󲿷֣��Ҳ���Ϊ��
        left = str;
        right.clear();
        return false;
    }
    return true;
}