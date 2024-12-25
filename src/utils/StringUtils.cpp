#include "utils/StringUtils.h"

#include <random>
#include <bitset>
#include <cstdlib>
#include <ctype.h>
#include <cstdio>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>


typedef std::chrono::system_clock SystemClock;
typedef std::chrono::high_resolution_clock HighResolutionClock;
typedef HighResolutionClock::time_point HighResolutionTimePoint;
typedef SystemClock::time_point SystemTimePoint;
typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::microseconds Microseconds;
typedef std::chrono::nanoseconds Nanoseconds;
typedef std::chrono::seconds Seconds;


size_t StringUtils::removeAllThatDoNotContain(std::vector<std::string> & input_strings,
                                        const std::string & input_string,
                                        bool is_case_sensitive)
{
    size_t totalRemoved = 0;
    int count = static_cast<int>(input_strings.size());
    if (count == 0 )
    {
        return 0;
    }

    for (int a1 = count-1;a1 >= 0; --a1)
    {
            if (!StringUtils::contains(input_strings[a1],input_string,is_case_sensitive))
            {
                input_strings.erase(input_strings.begin()+a1);
                ++totalRemoved;
            }
    }
    return totalRemoved;
}

void StringUtils::removeNonAlphaNumericInPlace(std::string & input_string)
{
    for (int a1 = static_cast<int>(input_string.size()) - 1; a1 >= 0; --a1)
    {
        if (!isalnum(input_string[a1]))
        {
            input_string.erase(input_string.begin()+a1);
        }
    }
}

std::string StringUtils::removeNonAlphaNumeric(const std::string &input_string)
{
    std::string str = input_string;
    removeNonAlphaNumericInPlace(str);
    return str;
}

std::pair<std::string,std::string> StringUtils::splitKeyValue(const std::string& input_string,
                                                              const std::string& delimiter_str,
                                                              bool trim_items)
{
    std::vector<std::string> item_pair = StringUtils::split(input_string,delimiter_str);
    if (item_pair.size() > 1)
    {
        if (trim_items)
        {
            return std::make_pair(StringUtils::trimmed(item_pair[0]),StringUtils::trimmed(item_pair[1]));
        }
        else
        {
            return std::make_pair(item_pair[0],item_pair[1]);
        }
    }
    return std::make_pair("","");
}

bool StringUtils::isEmpty(const std::string& input_string)
{
    return StringUtils::trimmed(input_string).empty();
}

bool StringUtils::isNumeric(const std::string &input_string, size_t number_base)
{
    std::istringstream iss( input_string.c_str() );

    if ( number_base == 10 )
    {
        double dTestSink;
        iss >> dTestSink;
    }
    else if ( number_base == 8 || number_base == 16 )
    {
        int nTestSink;
        iss >> ( ( number_base == 8 ) ? std::oct : std::hex ) >> nTestSink;
    }
    else return false;

    // was any input successfully consumed/converted?
    if ( ! iss )
    {
        return false;
    }

    // was all the input successfully consumed/converted?
    return ( iss.rdbuf()->in_avail() == 0 );
}

std::string StringUtils::join(const std::vector<std::string>& values,
                              const std::string& seperator_str,
                              const std::string& prefix_str,
                              const std::string& postfix_str)
{
    std::string out;
    size_t count = 0;
    size_t values_size = values.size();
    for (const auto& value : values)
    {
        out += prefix_str + value + postfix_str + (count < values_size - 1 ? seperator_str : "");
        ++count;
    }
    return out;
}

std::string StringUtils::join(const std::list<std::string>& values,
                                      const std::string& seperator_str,
                                      const std::string& prefix_str,
                                      const std::string& postfix_str)
{
  return join(std::vector<std::string>(values.begin(),values.end()),
                                       prefix_str,seperator_str,postfix_str);
}

std::string StringUtils::join(const std::set<std::string>& values,
                                     const std::string& seperator_str,
                                     const std::string& prefix_str,
                                     const std::string& postfix_str)
{
    return join(std::vector<std::string>(values.begin(),values.end()),
                                         prefix_str,seperator_str,postfix_str);
}

std::vector<std::string> StringUtils::createUniqueUUIDList(size_t count,
                                                           bool include_dashes)
{
    std::set<std::string> uuidSet;
    for (size_t a1 = 0; a1 < count; ++a1)
    {
        std::string uuid = createUUID(include_dashes);
        if (!uuidSet.count(uuid))
        {
              uuidSet.insert(uuid);
        }
        else
        {
            a1--;
            continue;
        }
    }
    return std::vector<std::string> (uuidSet.begin(), uuidSet.end());
}

std::string StringUtils::removeTextAfter(const std::string& input_string,
                                         const std::string& text_after_str,
                                         bool keep_text_after)
{
    std::vector<std::string> items = StringUtils::split(input_string,text_after_str);
    if (!items.empty())
    {
        return items[0] + (keep_text_after ? text_after_str : "");
    }
    return "";
}

bool StringUtils::multiequals(const std::string& input_string,
                        const std::vector<std::string> &comparisons,
                        bool is_case_sensitive)
{
    for (const auto &str : comparisons)
    {
        if (is_case_sensitive)
        {
            if (StringUtils::trimmed(str) == StringUtils::trimmed(input_string))
            {
                return true;
            }
        }
        else
        {
            if (StringUtils::equals(input_string,str))
            {
                return true;
            }
        }
    }
    return false;
}

void StringUtils::toUpperInPlace(std::string &input_string)
{
    std::transform(input_string.begin(), input_string.end(), input_string.begin(), ::toupper);
}


void StringUtils::toLowerInPlace(std::string &input_string)
{
    std::transform(input_string.begin(), input_string.end(), input_string.begin(), ::tolower);
}

std::string StringUtils::toLower(const std::string &input_string)
{
    std::string strOut = input_string;
    toLowerInPlace(strOut);
    return strOut;
}

std::string StringUtils::toUpper(const std::string &input_string)
{
    std::string strOut = input_string;
    toUpperInPlace(strOut);
    return strOut;
}

void  StringUtils::replaceInPlace(std::string &input_string,
                                  const std::string &old_str,
                                  const std::string &new_str,
                                  bool is_case_sensitive)
                            {
    if (input_string.empty() || old_str.empty())
    {
        return;
    }

    //Looks like user wants to remove a string
    if (new_str.empty())
    {
        removeInPlace(input_string,old_str,is_case_sensitive);
        return;
    }

    if (is_case_sensitive)
    {
        size_t pos = 0;
        while ((pos = input_string.find(old_str, pos)) != std::string::npos)
        {
            input_string.replace(pos, old_str.length(), new_str);
            pos += new_str.length();
        }
    }
    else
    {
        std::string subject = toUpper(input_string);
        std::string search = toUpper(old_str);
        std::string replace = toUpper(new_str);
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos)
        {
            input_string.replace(pos, search.length(), new_str);
            pos += new_str.length();
        }
    }
}

size_t StringUtils::removeInPlace(std::string &input_string,
                                  const std::string &str_to_remove,
                                  bool is_case_sensitive)
{
    if (input_string.empty() || str_to_remove.empty())
    {
        return 0;
    }
    size_t removeCount=0;
    if (is_case_sensitive)
    {
        std::string::size_type i = input_string.find(str_to_remove);

         while (i != std::string::npos)
        {
           input_string.erase(i, str_to_remove.length());
           ++removeCount;
           i = input_string.find(str_to_remove);

        }
    }
    else
    {

        std::string input_stringUpper = toUpper(input_string);
        std::string str_to_removeUpper = toUpper(str_to_remove);
        std::string::size_type i = input_stringUpper.find(str_to_removeUpper);

        while (i != std::string::npos)
        {
           input_string.erase(i, str_to_remove.length());
           input_stringUpper.erase(i, str_to_removeUpper.length());
           ++removeCount;
           i = input_stringUpper.find(str_to_removeUpper);
        }
    }
    return removeCount;
}


std::string StringUtils::formatWithThousandsLabel(const std::string &input_string)
{
    std::string numberStr = input_string;
    size_t decimalIndex = numberStr.find(".");
    std::string modifier;
    bool hasNumericModifier  = (numberStr.size() && !isdigit(numberStr[0]));
    if (hasNumericModifier)
    {
        modifier.push_back(numberStr[0]);
    }

    if (decimalIndex == std::string::npos)
    {
        decimalIndex = numberStr.size()-1;
    }
    else
    {
        decimalIndex--;
    }

    size_t counter = 0;
    int count = static_cast<int>(decimalIndex);
    for (int a1 = count; a1 >= 0; --a1)
    {
        counter++;
        if (counter == 3 && a1 != 0)
        {
            counter = 0;
            numberStr.insert(numberStr.begin() + a1, ',');
        }
    }

    if (hasNumericModifier)
    {
        StringUtils::replaceInPlace(numberStr,modifier+",",modifier);
    }
    return numberStr;
}

std::string StringUtils::toString(double input_float,
                                  int precision)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << input_float;
    std::string numberStr = stream.str();
    return numberStr;
}


std::string StringUtils::replace(const std::string &input_string,
                                 const std::string &old_str,
                                 const std::string &new_str,
                                 bool is_case_sensitive)
{

    if (is_case_sensitive)
    {
        std::string subject = input_string;
        std::string search  = old_str;
        std::string replace = new_str;

            size_t pos = 0;
            while((pos = subject.find(search, pos)) != std::string::npos)
            {
                 subject.replace(pos, search.length(), replace);
                 pos += replace.length();
            }
            return subject;
    }
    else
    {

        std::string original = input_string;
        std::string subject = toUpper(input_string);
        std::string search  = toUpper(old_str);

        size_t pos = subject.find(search);
        std::vector<size_t> indices;
        while(pos != std::string::npos)
        {
             indices.push_back(pos);
             subject.replace(pos, search.size(), new_str);
             pos = subject.find(search);
        }

        for (const auto &index : indices)
        {
            original.replace(index,old_str.size(),new_str);
        }

        return original;
    }
}

void StringUtils::trimmedInPlace(std::string &input_string)
{
    StringUtils::leftTrimmedInPlace(input_string);
    StringUtils::rightTrimmedInPlace(input_string);
}

void StringUtils::leftTrimmedInPlace(std::string &input_string)
{
    input_string.erase(input_string.begin(), std::find_if(input_string.begin(), input_string.end(), [](int ch)
    {
        return !std::isspace(ch);
    }));
}

void StringUtils::rightTrimmedInPlace(std::string &input_string)
{
    input_string.erase(std::find_if(input_string.rbegin(), input_string.rend(), [](int ch)
    {
        return !std::isspace(ch);
    }).base(), input_string.end());
}

std::string StringUtils::rightTrimmed(const std::string &input_string)
{
    std::string outputStr = input_string;
    rightTrimmedInPlace(outputStr);
    return outputStr;
}

std::string StringUtils::leftTrimmed(const std::string &input_string)
{
    std::string outputStr = input_string;
    leftTrimmedInPlace(outputStr);
    return outputStr;
}

std::string StringUtils::trimmed(const std::string &input_string)
{
    std::string str = input_string;
    trimmedInPlace(str);
    return str;
}

std::string StringUtils::toUpperTrimmed(const std::string &input_string)
{
    return trimmed(toUpper(input_string));
}

std::string StringUtils::toLowerTrimmed(const std::string &input_string)
{
    return trimmed(toLower(input_string));
}

bool StringUtils::equals(const std::string &input_string1,
                         const std::string &input_string2)
{
    return (toUpper(input_string1) == toUpper(input_string2));
}

void StringUtils::split(const std::string &input_string,
                        const std::string &delimiter_str,
                        std::vector<std::string> &split_items)
{
    std::string s = input_string;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter_str)) != std::string::npos)
    {
        token = s.substr(0, pos);
        split_items.push_back(token);
        s.erase(0, pos + delimiter_str.length());
    }
    split_items.push_back(s);
}

std::vector<std::string> StringUtils::split(const std::string &str,
                                            const std::string &delimiter)
{
    std::vector<std::string> outItems;
    split(str,delimiter,outItems);
    return outItems;
}

void StringUtils::multisplit(const std::string &input_string,
                             const std::vector<std::string>& delimiters,
                             std::vector<std::string>& output)
{
    size_t delim_counter = 0;
    bool splits_available = true;
    std::string curr_str = input_string;
    std::string next_str;
    while (splits_available)
    {
        std::string delimiter = delimiters[delim_counter];
        std::vector<std::string> items = StringUtils::split(curr_str,delimiter);
        splits_available = false;
        for(const auto& item: items)
        {
            if (!StringUtils::vectorContains(item,delimiters))
            {
                output.push_back(item);
            }
            else
            {
                splits_available = true;
                next_str += item;
            }
        }
        curr_str = next_str;
        next_str.clear();
        ++delim_counter;
        if (delim_counter >= delimiters.size())
        {
            delim_counter = 0;
        }
    }
}

std::vector<std::string> StringUtils::multisplit(const std::string &input_string,
                                                 const std::vector<std::string> &delimiters)
{
    std::vector<std::string> output;
    multisplit(input_string,delimiters,output);
    return output;
}

bool StringUtils::contains(const std::string &input_string,
                           const std::string &input_sub_str,
                           bool is_case_sensitive)
{
    if (is_case_sensitive)
    {
        if (input_string.find(input_sub_str) != std::string::npos)
        {
            return true;
        }
    }
    else
    {
        if (toUpper(input_string).find(toUpper(input_sub_str)) !=
                std::string::npos)
        {
            return true;
        }
    }
    return false;
}

bool StringUtils::vectorContains(const std::vector<std::string> & input_strings,
                                 const std::string & input_sub_str,
                                 bool is_case_sensitive)
{
    for (size_t a1 = 0; a1 < input_strings.size(); ++a1)
    {
        bool value = StringUtils::contains(input_strings[a1],input_sub_str,is_case_sensitive);
        if (value)
        {
            return true;
        }
    }
    return false;
}

bool StringUtils::vectorContains(const std::string & input_string,
                                 const std::vector<std::string> & input_sub_strings,
                                 bool is_case_sensitive)
{
    for (size_t a1 = 0; a1 < input_sub_strings.size(); ++a1)
    {
        bool exists = StringUtils::contains(input_string,input_sub_strings[a1],is_case_sensitive);
        if (exists)
        {
            return true;
        }
    }
    return false;
}


size_t StringUtils::indexOf(const std::string &input_string,
                            const std::vector<std::string> & items,
                            bool is_case_sensitive)
{
    if (items.size() == 0)
    {
        return std::string::npos;
    }


    if (is_case_sensitive)
    {
        for (size_t a1 = 0; a1 < items.size(); ++a1)
        {
            if (items[a1] == input_string)
            {
                return a1;
            }
        }
    }
    else
    {
        for (size_t a1 = 0; a1 < items.size(); ++a1)
        {
            if (StringUtils::equals(items[a1],input_string))
            {
                return a1;
            }
        }
    }
    return std::string::npos;
}

std::string StringUtils::textAfter(const std::string &input_string,
                                   const std::string &input_text_after_delimiter,
                                   bool  is_case_sensitive)
{
    if (input_string.empty() || input_text_after_delimiter.empty())
    {
        return "";
    }
    std::string str=input_string;
    std::string v1=input_text_after_delimiter;
    if (!is_case_sensitive)
    {
        str=toUpper(str);
        v1=toUpper(v1);
    }

    size_t start = str.find(v1);
    if (start==std::string::npos)
    {
        return "";
    }

    start=start+v1.size();
    size_t len=str.size()-v1.size();
    return input_string.substr(start,len);
}

std::string StringUtils::textBefore(const std::string &input_string,
                                    const std::string &input_sub_string,
                                    bool is_case_sensitive)
{
    std::string str=input_string;
    std::string v1=input_sub_string;
    if (!is_case_sensitive)
    {
        str=toUpper(str);
        v1=toUpper(v1);
    }

    size_t len = str.find(v1);
    if (len == std::string::npos)
    {
        return "";
    }
    return input_string.substr(0,len);
}

std::string StringUtils::repeat(const std::string &input_string,
                                size_t repeat_count)
{
    std::string return_string;
    repeat(input_string,return_string,repeat_count);
    return return_string;
}

void StringUtils::repeat(const std::string &input_string,
                         std::string & return_string,
                         size_t repeat_count)
{
    for (size_t a1 = 0; a1 < repeat_count; ++a1)
    {
        return_string += input_string;
    }
}

std::string StringUtils::remove(const std::string &input_string,
                          const std::string &str_to_remove,
                          bool is_case_sensitive)
{
    std::string source = input_string;
    removeInPlace(source,str_to_remove,is_case_sensitive);
    return source;
}

std::string StringUtils::multiremove(const std::string &input_string,
                               const std::vector<std::string> &input_strings_to_remove,
                               bool is_case_sensitive)
{
    std::string strOut = input_string;;
    for (const auto &str_to_remove : input_strings_to_remove)
    {
        strOut = StringUtils::remove(strOut,str_to_remove,is_case_sensitive);
    }
    return strOut;
}

std::uint64_t StringUtils::toUInt(const std::string &input_string)
{
    return toNumber<std::uint64_t>(input_string);
}

std::int64_t StringUtils::toInt(const std::string &input_string)
{
    return toNumber<std::int64_t>(input_string);
}

double StringUtils::toFloat64(const std::string &input_string)
{
   return toNumber<double>(input_string);
}

float StringUtils::toFloat32(const std::string &input_string)
{
    return toNumber<float>(input_string);
}

bool StringUtils::isFloat(const std::string &input_string)
{
    std::istringstream iss(input_string);
    double f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

bool StringUtils::startsWith(const std::string &input_string,
                             const std::string &starts_with_input_string,
                             bool is_case_sensitive)
{
    if(input_string.size()<starts_with_input_string.size())
    {
        return false;
    }

    if (is_case_sensitive)
    {
        return !input_string.compare(0, starts_with_input_string.size(), starts_with_input_string);
    }
    else
    {
        std::string str1=toLower(input_string);
        std::string str2=toLower(starts_with_input_string);
        return !str1.compare(0, str2.size(), str2);
    }
}

bool StringUtils::endsWith(const std::string &input_string,
                           const std::string &ends_with_input_string,
                           bool is_case_sensitive)
{
    if(input_string.size()<ends_with_input_string.size())
    {
       return false;
    }
    if (is_case_sensitive)
    {
       return !input_string.compare(input_string.size() - ends_with_input_string.size(), ends_with_input_string.size(), ends_with_input_string);
    }
    else
    {
       std::string srcLower=toLower(input_string);
       std::string valueLower = toLower(ends_with_input_string);
       return !srcLower.compare(srcLower.size() - valueLower.size(), valueLower.size(), valueLower);
    }
}

bool StringUtils::endsWithMultiple(const std::string & input_string,
                                   const std::vector<std::string> & ends_with_input_strings,
                                   bool is_case_sensitive)
{
    for(const std::string & item : ends_with_input_strings)
    {
        if (endsWith(input_string,item,is_case_sensitive))
        {
            return true;
        }
    }
    return false;
}

void StringUtils::vectorErase(std::vector<std::string> &input_string_vector,
                              const std::string &input_string_to_erase,
                              bool is_case_sensitive)
{
    if (is_case_sensitive)
    {
        input_string_vector.erase(std::remove(input_string_vector.begin(),
                                              input_string_vector.end(),
                                              input_string_to_erase),
                                              input_string_vector.end());
    }
    else
    {
        std::string value = toUpper(input_string_to_erase);
        int count = static_cast<int>(input_string_vector.size());
        for (int a1 = count - 1; a1 >= 0; --a1)
        {
            if (value == toUpper(input_string_vector[a1]))
            {
                input_string_vector.erase(input_string_vector.begin()+a1);
            }
        }
    }
}

void StringUtils::findAllThatStartWith(const std::vector<std::string> &input_string_vector,
                                       const std::string &starts_with_input_string,
                                       std::vector<std::string> &output_results,
                                       bool is_case_sensitive)
{
    for (size_t a1 = 0; a1 < input_string_vector.size(); ++a1)
    {
        if (startsWith(input_string_vector[a1],starts_with_input_string,is_case_sensitive))
        {
            output_results.push_back(input_string_vector[a1]);
        }
    }
}

void StringUtils::findAllThatEqual(const std::vector<std::string> &input_string_vector,
                                   const std::string &matching_value_input_string,
                                   std::vector<std::string> &output_results,
                                   bool is_case_sensitive)
{
    if (is_case_sensitive)
    {
        for (size_t a1 = 0; a1 < input_string_vector.size(); ++a1)
        {
            if (input_string_vector[a1] == matching_value_input_string)
            {
                output_results.push_back(input_string_vector[a1]);
            }
        }
    }
    else
    {
        std::string value = StringUtils::toUpper(matching_value_input_string);
        for (size_t a1 = 0; a1 < input_string_vector.size(); ++a1)
        {
            if (StringUtils::toUpper(input_string_vector[a1]) == value)
            {
                output_results.push_back(input_string_vector[a1]);
            }
        }
    }
}

void StringUtils::findAllThatEndWith(const std::vector<std::string> &items,
                                     const std::string &endsWithValue,
                                     std::vector<std::string> &itemsOut,
                                     bool is_case_sensitive)
{
    for (size_t a1 = 0; a1 < items.size(); ++a1)
    {
        if (endsWith(items[a1],endsWithValue,is_case_sensitive))
        {
            itemsOut.push_back(items[a1]);
        }
    }
}

void StringUtils::findAllThatContain(const std::vector<std::string> &items,
                                     const std::string &containsValue,
                                     std::vector<std::string> &itemsOut,
                                     bool is_case_sensitive)
{
    for (size_t a1 = 0; a1 < items.size(); ++a1)
    {
        if (contains(items[a1],containsValue,is_case_sensitive))
        {
            itemsOut.push_back(items[a1]);
        }
    }
}

std::string StringUtils::textBetween(const std::string &source,
                                     const std::string &firstValue,
                                     const std::string &lastValue,
                                     bool  is_case_sensitive)
{
    std::string str=source;
    std::string v1=firstValue;
    std::string v2=lastValue;
    if (!is_case_sensitive)
    {
        str=toUpper(str);
        v1=toUpper(v1);
        v2=toUpper(v2);
    }

    size_t start = str.find(v1);
    size_t end   = str.find(v2, start+v1.length());
    if(start == std::string::npos || end == std::string::npos)
    {
        return "";
    }
    return source.substr(start+v1.length(), end - (start+v1.length()));
}

void StringUtils::clean(std::vector<std::string> & input_strings)
{
    if (input_strings.size() == 0)
    {
        return;
    }
    int lastIndex = static_cast<int>(input_strings.size()) - 1;
    for (int a1 = lastIndex; a1>=0 ;--a1)
    {
        size_t index = static_cast<size_t>(a1);
        if (input_strings[index].empty())
        {
            input_strings.erase(input_strings.begin()+index);
            continue;
        }
        StringUtils::trimmedInPlace(input_strings[index]);
    }
}

std::string StringUtils::createUUID(bool include_dashes)
{
    std::string uuid;    
    std::random_device rng_device;
    std::mt19937 rng_engine(rng_device());
    std::uniform_int_distribution<int> ran_percent(0,100);
    std::uniform_int_distribution<char> ran_dec('0','9');
    std::uniform_int_distribution<char> ran_alp('A','F');

    for (size_t a1 = 0; a1 < 20; ++a1)
    {
        uuid.push_back((ran_percent(rng_engine) > 50 ? ran_dec(rng_engine) : ran_alp(rng_engine) ));
    }

    HighResolutionTimePoint tp = HighResolutionClock::now();
    Milliseconds secs= std::chrono::duration_cast<Milliseconds>(tp.time_since_epoch());
    std::uint64_t ms = static_cast<std::uint64_t>(secs.count());

    std::string timeStr = StringUtils::toString(ms);
    if (timeStr.size()>12)
    {
        timeStr=timeStr.substr(0,12);
    }
    else
    {
        for (size_t a1 = timeStr.size(); a1<12;++a1)
        {
            timeStr.push_back('0');
        }
    }
    uuid+=timeStr;
    if (include_dashes)
    {

        uuid.insert(20,"-");
        uuid.insert(16,"-");
        uuid.insert(12,"-");
        uuid.insert(8,"-");
    }

    return uuid;
}

std::vector<size_t> StringUtils::getWholeWords(const std::string &input_string,
                                               const std::string &whole_word_string)
{
    std::vector<size_t> wholeWords;

    auto index = input_string.find(whole_word_string,0);
    while(index != std::string::npos)
    {
        bool backIsAlphaNum = false;
        bool frontIsAlphaNum = false;
        if (index + whole_word_string.size() < input_string.size() && isalnum(input_string[index+whole_word_string.size()]))
        {
            backIsAlphaNum = true;
        }

        if (index > 0 && isalnum(input_string[index-1]))
        {
            frontIsAlphaNum = true;
        }
        if (!backIsAlphaNum && !frontIsAlphaNum)
        {
            wholeWords.push_back(index);
        }
        index = input_string.find(whole_word_string,index+1);
    }
    return wholeWords;
}

std::vector<std::uint64_t> StringUtils::serializeToIntegerList(const std::string &input_string)
{
    std::vector<std::uint64_t> serialized;
    for (size_t a1 = 0;a1< input_string.size();++a1)
    {
        serialized.push_back(static_cast<std::uint64_t>(input_string[a1]));
    }
    return serialized;
}

bool StringUtils::isUnsignedInteger(const std::string &input_string)
{
    for (size_t a1 = 0;a1<input_string.length();++a1)
    {
        if (isalpha(input_string[a1])  || input_string[a1]=='.' )
        {
            return false;
        }
    }

    return true;
}

bool StringUtils::isSignedInteger(const std::string &input_string)
{
    for (size_t a1 = 0; a1 < input_string.length(); ++a1)
    {
        if (isalpha(input_string[a1]) || input_string[a1]=='.')
        {
            if (input_string[a1]=='-')
            {
                continue;
            }
            return false;
        }
    }
    return true;
}

std::vector<std::string> StringUtils::vectorCombine(const std::vector<std::string> &vecFirst,
                                                    const std::vector<std::string> &input_strings_second)
{
    std::vector<std::string> vecNew = vecFirst;
    vecNew.insert(vecNew.end(),input_strings_second.begin(),input_strings_second.end());
    return vecNew;
}

size_t StringUtils::removeAllThatDoNotEndWith(std::vector<std::string> & input_strings,
                                        const std::string & input_string_to_remove,
                                        bool is_case_sensitive)
{
    size_t totalRemoved = 0;
    int count = static_cast<int>(input_strings.size());
    if (count == 0 )
    {
        return 0;
    }

    for (int a1 = count-1;a1 >= 0; --a1)
    {
        if (!StringUtils::endsWith(input_strings[a1],input_string_to_remove,is_case_sensitive))
        {
            input_strings.erase(input_strings.begin() + a1);
            ++totalRemoved;
        }
    }
    return totalRemoved;
}

size_t StringUtils::removeAllThatEndWith(std::vector<std::string> & input_strings,
                                         const std::string & input_string_to_remove,
                                         bool is_case_sensitive)
{
    size_t totalRemoved = 0;
    int count = static_cast<int>(input_strings.size());
    if (count == 0 )
    {
        return 0;
    }

    for (int a1 = count-1;a1 >= 0; --a1)
    {
            if (StringUtils::endsWith(input_strings[a1],input_string_to_remove,is_case_sensitive))
            {
                input_strings.erase(input_strings.begin()+a1);
                ++totalRemoved;
            }
    }
    return totalRemoved;
}

size_t StringUtils::removeAllThatStartWith(std::vector<std::string> & input_strings,
                                           const std::string & input_string_to_remove,
                                           bool is_case_sensitive)
{
    size_t totalRemoved = 0;
    int count = static_cast<int>(input_strings.size());
    if (count == 0 )
    {
        return 0;
    }

    for (int a1 = count-1;a1 >= 0; --a1)
    {
            if (StringUtils::startsWith(input_strings[a1],input_string_to_remove,is_case_sensitive))
            {
                input_strings.erase(input_strings.begin()+a1);
                ++totalRemoved;
            }
    }
    return totalRemoved;
}

size_t StringUtils::removeAllThatDoNotStartWith(std::vector<std::string> & input_strings,
                                                const std::string & input_string_to_remove,
                                                bool is_case_sensitive)
{
    size_t totalRemoved = 0;
    int count = static_cast<int>(input_strings.size());
    if (count == 0 )
    {
        return 0;
    }

    for (int a1 = count-1;a1 >= 0; --a1)
    {
            if (!StringUtils::startsWith(input_strings[a1],input_string_to_remove,is_case_sensitive))
            {
                input_strings.erase(input_strings.begin()+a1);
                ++totalRemoved;
            }
    }
    return totalRemoved;
}

size_t StringUtils::removeAllThatContain(std::vector<std::string> & input_strings,
                            const std::string & input_string_to_remove,
                            bool is_case_sensitive)
{
    size_t totalRemoved = 0;
    int count = static_cast<int>(input_strings.size());
    if (count == 0 )
    {
        return 0;
    }

    for (int a1 = count-1;a1 >= 0; --a1)
    {
            if (StringUtils::contains(input_strings[a1],input_string_to_remove,is_case_sensitive))
            {
                input_strings.erase(input_strings.begin()+a1);
                ++totalRemoved;
            }
    }
    return totalRemoved;
}
