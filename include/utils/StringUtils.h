#pragma once

//std headers
#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <iomanip>
#include <sstream>
#include <bitset>

#include "UtilityCommon.hpp"

class StringUtils
{
public:
	/**
	* @brief Converts wide string to upper-case
    * @param input_string Reference to the string to convert
	*/
    static void toUpperInPlace(std::string &input_string);

    /**
    * @brief Converts string to lower-case
    * @param input_string Reference to the string to convert
    */
    static void toLowerInPlace(std::string &input_string);

	/**
    * @brief Converts string to lower-case
    * @param input_string The string to convert
	* @return The converted string
	*/
    static std::string toLower(const std::string &input_string);

    /**
    * @brief Converts string to upper-case
    * @param input_string The string to convert
    * @return The converted string
    */
    static std::string toUpper(const std::string &input_string);

	/**
	* @brief Perfroms string replace (in-place)
    * @param input_string Reference to the input_string to perform
	* in-place replacement
    * @param old_str The old string to replace
    * @param new_str The new string to insert
    * @param is_case_sensitive Should old_str be is_case_sensitive
    * within input_string?
	*/
    static void replaceInPlace(std::string &input_string,
                               const std::string &old_str,
                               const std::string &new_str,
                               bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);


	/**
	* @brief Remove all instances of a substring from a string
    * @param input_string The string to remove characters from
    * @param str_to_remove The actual string to remove
    * @param is_case_sensitive Case sensitivity
    * @return Number of removes performed on input_string
	*/
    static size_t removeInPlace(std::string &input_string,
                                const std::string &str_to_remove,
                                bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

	/**
     * @brief toHexString Converts integer value into hexadecimal string
     * @param value The value to convert
     * @return Return std::string of value
	 */
	template <typename T>
    static std::string toHexString(T value);

	/**
	 * @brief Returns a bitstring representing template value
     * @param input_value
     * @return Return string containing 64 bit binary input_value
	 */
	template <typename T>
    static std::string toBitString(T input_value);

    /**
     * @brief toNumber Convert string to number
     * @param input_value
     * @return Return converted number
     */
    template<typename T>
    static T toNumber(const std::string &input_value);


	/**
	 * @brief Prints vector of templated type if underlying type
	 *        supports it.
	 * @param out The stream to write to
	 * @param values The values to print
	 */
	template <typename T>
    static void printVector(std::ostream & out,
                            const std::vector<T> & values);

    /**
     * @brief formatWithThousandsLabel Adds comma in correct spot for numeric string.
     * e.g 1000 = 1,000 and 123456 = 123,456
     * @param input_string The number string to process
     * @return Return converted string with commas or original input_string
     */
    static std::string formatWithThousandsLabel(const std::string &input_string);


	/**
     * @brief toString Method provided for legacy support.
     * New code could directly use std::to_string
	 * @param value
     * @return Return string representation of value
	 */
	template <typename T>
    static std::string toString(T value);

	/**
	* @brief Converts floating point values to "x.xxxx" form
    * @param input_float Floating point value
    * @param precision Number of digits to include after decimal
    * @return Return string of converted floating point value
	*/
    static std::string toString(double input_float,
                                int precision = 6);


	/**
	 * @brief Perfroms string replace (in-place)
     * @param input_string The string that contains old_str to replace
     * @param old_str The old string to replace
     * @param new_str The new string to insert
     * @param is_case_sensitive Should old_str be is_case_sensitive
     * within input_string?
     * @return input_string with the replaced old_str.  If old_str
     * is not found within input_string then input_string is returned
	 * unmodified.
	 */
    static std::string replace(const std::string &input_string,
                                     const std::string &old_str,
                                     const std::string &new_str,
                                     bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

    /**
     * @brief leftTrimmedInPlace Trims whitespace from left of string
     * @param input_string Reference to the string to process
     */
    static void leftTrimmedInPlace(std::string &input_string);

    /**
     * @brief trimmedInPlace Trims characters from string
     * @param input_string The input string to trim
     * @param custom_character_to_trim Character to remove from input_string
     * @param trim_all_characters Should all characters be trimmed that
     * match custom_character_to_trim
     */
    static void trimmedInPlace(std::string &input_string);

    /**
     * @brief rightTrimmedInPlace Trims whitespace from right of string
     * @param input_string Reference to the string to process
     */
    static void rightTrimmedInPlace(std::string &input_string);

    /**
     * @brief rightTrimmed Trims from right of string
     * @param stringToTrim Read only string to process
     * @return Returns a copy of the trimmed input_string
     */
    static std::string rightTrimmed(const std::string &input_string);

    /**
     * @brief leftTrimmed Trims from left of string
     * @param stringToTrim Read only string to process
     * @return Returns a copy of the trimmed input_string
     */
    static std::string leftTrimmed(const std::string &input_string);

    /**
     * @brief trimmed Trims left and right end of a string
     * @param input_string Read only input string to process
     * @return Return copy of input_string that has been trimmed
     * from left and right
     */
    static std::string trimmed(const std::string &input_string);

	/**
    * @brief Converts string to upper-case and then trims it
	* @param input The string to convert
	* @return The converted string
	*/
    static std::string toUpperTrimmed(const std::string &input_string);

	/**
    * @brief Converts string to lower-case and then trims it
    * @param input_string The string to convert
	* @return The converted string
	*/
    static std::string toLowerTrimmed(const std::string &input_string);

	/**
	* @brief Determines if two (case-insensitive) strings are equal
    * @param input_string1 The first string
    * @param input_string2 The second string
	* @return True if case-insensitve match occurs, false otherwise
	*/
    static bool equals(const std::string &input_string1,
                       const std::string &input_string2);

	/**
	* @brief Splits a string based on the delimeter
    * @param input_string The string to split
	* @param delimiter The delimiter used to break the string apart
	* @param splitItems Reference to string vector. Avoids vector copy.
	* @code {.cpp}
		//Example: Split string based on custom delim

		std::string line = "test|item <$CBTEK$> test,data";

		std::vector<std::string>items;

		split(line,"<$CBTEK$>",items);

		//Example Results:
		//items[0]="test|item";
		//items[1]="test,data";
		@endcode
	*/
    static void split(const std::string &input_string,
                      const std::string &delimiter_str,
                      std::vector<std::string> &split_items);


	/**
	* @brief Splits a string based on the delimeter
    * @param input_string The string to split
    * @param delimiter_str The delimiter used to break the string apart
	* @return Return a vector of split strings
	* @code {.cpp}
		//Example: Split string based on custom delim

		std::string line = test|item <$CBTEK$> test,data

		std::vector<std::string>items;

		items = split(line,"<$CBTEK$>");

		//Example Results:
		//items[0]="test|item";
		//items[1]="test,data";
		@endcode
	*/
    static std::vector<std::string> split(const std::string &input_string,
                                          const std::string &delimiter_str);


    /**
     * @see see std::vector<std::string> split(const std::string &str,
                                         const std::vector<std::string> &delimiters)
     */
    static void multisplit(const std::string &input_string,
                           const std::vector<std::string>& delimiters,
                           std::vector<std::string>& output);

    /**
    * @brief Splits a string based on several delimeters
    * @param input_string The string to split
    * @param delimiters Vector of delimiters used to break the string apart
    * @return Return a vector of split strings
    * @code {.cpp}

        //Example: Split string based on custom delims
        std::string line = test|item <$CBTEK$> test,data
        std::vector<std::string>items;
        std::vector<std::string> delims;
        delims.push_back("|");
        delims.push_back(",");
        delims.push_back("<$CBTEK$>");
        items = split(line,delims);

        //Example Results:
        //items[0]="test";
        //items[1]="item";
        //items[2]="test";
        //items[3]="data";
        @endcode
    */
    static std::vector<std::string> multisplit(const std::string &input_string,
                                               const std::vector<std::string> &delimiters);

    /**
     * @brief contains Determines if a substring exists within the input_stringing
     * @param input_string The source string to process
     * @param input_sub_str The substring to look for
     * @param is_case_sensitive
     * @return Return true if input_sub_str was found, false otherwise
     */
    static bool contains(const std::string &input_string,
                         const std::string &input_sub_str,
                         bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);


    /**
     * @brief vectorContains Determines if a substring exists within string of each item in vector
     * @param input_strings Vector containing list of strings
     * @param input_sub_str The substring to look for
     * @param is_case_sensitive Should comparisons be case sensitive?
     * @return Return true if input_sub_str was found, false otherwise
     */
    static bool vectorContains(const std::vector<std::string> & input_strings,
                         const std::string & input_sub_string,
                         bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);



    /**
     * @brief vectorContains Determines if any item in list of substrings
     * is contained within the given input string.
     * @param input_string The source string
     * @param input_sub_strings A vector of sub strings
     * @param is_case_sensitive Should comparisons be case sensitive?
     * @return Return true if any of the substrings exist in input_string, false otherwise
     */
    static bool vectorContains(const std::string & input_string,
                               const std::vector<std::string> & input_sub_strings,
                               bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);


    /**
    * @brief join Joins all items in vector together and formats each item with
    * optional seperator, prefix and postfix string
    * @param input_strings Set of items to join together
    * @param seperator_string Seperator string to insert between each item in
    * the StringList
    * @param prefix_string Prefix string that is inserted before each item in
    * the StringList
    * @param postfix_string Postfix string that is inserted after each item in
    * the StringList
    * @return Concatenated string of all the contents
    * seperated by "seperator" string and surrounded by
    * "prefix" and "postfix" strings
    */
    static std::string join(const std::vector<std::string> & input_strings,
                            const std::string &seperator_string = "",
                            const std::string &prefix_string = "",
                            const std::string &postfix_string = ""
                            );

    /**
    * @brief join Joins all items in set together and formats each item with
    * optional seperator, prefix and postfix string
    * @param input_strings Set of items to join together
    * @param seperator_string Seperator string to insert between each item in
    * the StringList
    * @param prefix_string Prefix string that is inserted before each item in
    * the StringList
    * @param postfix_string Postfix string that is inserted after each item in
    * the StringList
    * @return Concatenated string of all the contents
    * seperated by "seperator" string and surrounded by
    * "prefix" and "postfix" strings
    */
    static std::string join(const std::set<std::string> & input_strings,
                            const std::string &seperator_string = "",
                            const std::string &prefix_string = "",
                            const std::string &postfix_string = ""
                            );

    /**
    * @brief join Joins all items in list together and formats each item with
    * optional seperator, prefix and postfix string
    * @param input_strings Set of items to join together
    * @param seperator_string Seperator string to insert between each item in
    * the StringList
    * @param prefix_string Prefix string that is inserted before each item in
    * the StringList
    * @param postfix_string Postfix string that is inserted after each item in
    * the StringList
    * @return Concatenated string of all the contents
    * seperated by "seperator" string and surrounded by
    * "prefix" and "postfix" strings
    */
    static std::string join(const std::list<std::string> & input_strings,
                            const std::string &seperator_string = "",
                            const std::string &prefix_string = "",
                            const std::string &postfix_string = ""
                            );
    /**
    * @brief Returns the index of the given input_string
    * @param input_string The string to search
    * @param items Vector of items to search
    * @param is_case_sensitive Case sensitivity of the input_string
    * @return Return valid index >= 0 or npos if input_string is not in items vector
	*/
    static size_t indexOf(const std::string &input_string,
						  const std::vector<std::string> & items,
                          bool is_case_sensitive);

	/**
	* @brief Find value and return all text after it(exclusive)
    * @param input_string The string to copy and operate upon
    * @param input_text_after_delimiter The string delimiter to look for
    * @param is_case_sensitive Should value be case sensitive?
    * @return Returns string containing the text found after input_text_after_delimiter
	*/
    static std::string textAfter(const std::string &input_string,
                                 const std::string &input_text_after_delimiter,
                                 bool  is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);


	/**
    * @brief textBefore Find value and return all text before it(exclusive)
    * @param input_string The string to copy and operate upon
    * @param input_sub_string The string to look for
    * @param is_case_sensitive Should value be case sensitive?
    * @return Return the text found before value
	*/
    static std::string textBefore(const std::string &input_string,
                                 const std::string &input_sub_string,
                                 bool  is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

	/**
    * @brief repeat (Convience Function) Repeats a string a given number of times
    * @param input_string The string to repeat
    * @param repeat_count The number of times to repeat string
    * @return Return new string containing repeated input_string
	*/
    static std::string repeat(const std::string &input_string,
                              size_t repeat_count);


    /**
    * @brief repeat Repeats a string a given number of times
    * @param input_string The string to repeat
    * @param return_string New string containing repeated input_string
    * @param repeat_count The number of times to repeat string
    */
    static void repeat(const std::string &input_string,
                       std::string & return_string,
                       size_t repeat_count);


	/**
     * @brief remove Remove all occurences of "str_to_remove" from "input_string"
     * @param input_string The source string to process
     * @param str_to_remove The string that needs to be removed
     * @param is_case_sensitive Should check for str_to_remove be case sensitive?
     * @return Return a new string with removed str_to_remove or input_string
     * if str_to_remove was not found
	 */
    static std::string remove(const std::string &input_string,
                              const std::string &str_to_remove,
                              bool is_case_sensitive=c_DEFAULT_CASE_SENSITIVE);

    /**
     * @brief multiremove Remove multiple strings from input_string
     * @param input_string The source string to process
     * @param input_strings_to_remove Vector of strings to remove
     * @param is_case_sensitive Should string comparisions be case sensitive?
     * @return Return new string with removed items
     */
    static std::string multiremove(const std::string &input_string,
                                   const std::vector<std::string> &input_strings_to_remove,
                                   bool is_case_sensitive=c_DEFAULT_CASE_SENSITIVE);

    /**
     * @brief toUInt Convert string to 64-bit unsigned integer value
     * @param input_string The input string containing the unsigned integer value
     * @return Return converted unsigned integer value
     */
    static std::uint64_t toUInt(const std::string &input_string);

    /**
     * @brief toInt Convert string to 64-bit integer value
     * @param input_string The input string containing the integer value
     * @return Return converted integer value
     */
    static std::int64_t toInt(const std::string &input_string);

    /**
     * @brief toFloat64 Convert string to 64-bit floating point value
     * @param input_string The input stringing containing the floating point value
     * @return Return converted float value
     */
    static double toFloat64(const std::string &input_string);

    /**
     * @brief toFloat32 Convert string to 32-bit floating point value
     * @param input_string The input stringing containing the floating point value
     * @return Return converted float value
     */
    static float toFloat32(const std::string &input_string);

    /**
     * @brief isFloat Determine if input string can be converted to floating point value
     * @param input_string The input stringing containing the floating point value
     * @return True if string is a float, false otherwise
     */
    static bool isFloat(const std::string &input_string);

	/**
    * @brief Determines if input_string starts with value
    * @param input_string The source input string
    * @param starts_with_input_string The value that input_string startsWith
    * @param is_case_sensitive Should startsWithValue be case sensitive?
    * @return true if input_string starts with startsValue, else false
	*/
    static bool startsWith(const std::string &input_string,
                           const std::string &starts_with_input_string,
                           bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

    /**
    * @brief endsWith Determines if input_string ends with a certain value
    * @param input_string The source input string
    * @param ends_with_input_string The ends_with string
    * @param is_case_sensitive Should ends_with_input_string be case sensitive?
    * @return true if input_string ends with ends_with_input_string, else false
    */
    static bool endsWith(const std::string &input_string,
                         const std::string &ends_with_input_string,
                         bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

	/**
     * @brief endsWithMultiple Tests whether the input_string ends with any item from
     * the ends_with_input_strings vector
     * @param input_string The source string to test
     * @param ends_with_input_strings List of possible strings that input_string could end with
     * @param is_case_sensitive
     * @return Return true if any item from ends_with_input_strings occurs at the end of input_string
	 */
    static bool endsWithMultiple(const std::string &input_string,
                                 const std::vector<std::string> & ends_with_input_strings,
                                 bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

    /**
     * @brief vectorErase Erases all occurences of input_string_to_erase from
     * inpnut_string_vector
     * @param input_string_vector Reference to source vector to erase from
     * @param input_string_to_erase The string to erase from input_string_vector
     * @param is_case_sensitive Should input_string_to_erase be case sensitive?
     */
    static void vectorErase(std::vector<std::string> &input_string_vector,
                            const std::string &input_string_to_erase,
                            bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);
    /**
     * @brief findAllThatStartWith
     * @param input_string_vector
     * @param starts_with_input_string
     * @param output_results
     * @param is_case_sensitive
     */
    static void findAllThatStartWith(const std::vector<std::string> &input_string_vector,
                                     const std::string &starts_with_input_string,
                                     std::vector<std::string> &output_results,
                                     bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

    /**
     * @brief findAllThatEqual
     * @param input_string_vector
     * @param matching_value_input_string
     * @param output_results
     * @param is_case_sensitive
     */
    static void findAllThatEqual(const std::vector<std::string> &input_string_vector,
                            const std::string &matching_value_input_string,
                            std::vector<std::string> &output_results,
                            bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

	/**
	 * @brief findAllThatEndWith
	 * @param items
	 * @param startsWithValue
	 * @param itemsOut
     * @param is_case_sensitive
	 */
    static void findAllThatEndWith(const std::vector<std::string> &input_string_vector,
							const std::string &endsWithValue,
							std::vector<std::string> &itemsOut,
                            bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);
	/**
	 * @brief findAllThatContain
	 * @param items
	 * @param startsWithValue
	 * @param itemsOut
     * @param is_case_sensitive
	 */
    static void findAllThatContain(const std::vector<std::string> &items,
								   const std::string &containsValue,
								   std::vector<std::string> &itemsOut,
                                   bool is_case_sensitive=c_DEFAULT_CASE_SENSITIVE);
	/**
	* @brief Find and return all text between firstValue and lastValue (exclusive)
    * @param input_string The string to copy and operate upon
	* @param firstValue The begin string to find
	* @param lastValue The end string value
    * @param is_case_sensitive Should firstValue and lastValue be case sensitive?
	* @return The text found between first and last.  Empty string
	* is returned if both first and last are not found.
	*/
    static std::string textBetween(const std::string &source,
								   const std::string &firstValue,
								   const std::string &lastValue,
                                   bool  is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

	/**
	 * @brief Trims all items and removes empty elements
	 * @param items
	 */
    static void clean(std::vector<std::string> & input_strings);
	
    /**
     * @brief createUUID Generates a random UUID (GUID)
     * @param include_dashes Should dashes be included in output string?
     * @return Return string containing random uuid
     */
    static std::string createUUID(bool include_dashes = false);

    /**
     * @brief getWholeWords Return a list of whole word indices from a string
     * @param input_string The source string to process
     * @param whole_word_string The whole world to search for
     * @return Return vector of indices to each whole word occurence in the input_string
     */
    static std::vector<size_t> getWholeWords(const std::string &input_string, const std::string &whole_word_string);

    /**
     * @brief serializeToIntegerList Converts each character in input_string into an integer
     * @param input_string The string to convert
     * @return Return vector of unsigned integers representing each character in input_string
     */
    static std::vector<std::uint64_t> serializeToIntegerList(const std::string &input_string);

    /**
     * @brief isUnsignedInteger Determines if input_string can be converted to unsigned integer
     * @param input_string The string to test
     * @return True if string can be converted, false otherwise
     */
    static bool isUnsignedInteger(const std::string &input_string);

    /**
     * @brief isSignedInteger Determines if input_string can be converted to signed integer
     * @param input_string The string to test
     * @return True if string can be converted, false otherwise
     */
    static bool isSignedInteger(const std::string &input_string);

    /**
     * @brief vectorCombine Takes to vectors and combines them together
     * @param input_strings_first The first vector to combine
     * @param input_strings_second The second vector to combine
     * @return Return new vector containing combined first and second vectors
     */
    static std::vector<std::string> vectorCombine(const std::vector<std::string> &input_strings_first,
                                                  const std::vector<std::string> &input_strings_second);

    /**
     * @brief removeAllThatDoNotEndWith Removes all items from input_strings that
     * DO NOT end with input_string_to_remove
     * @param input_strings Reference to vector of strings
     * @param input_string_to_remove The string to remove
     * @param is_case_sensitive Should check be case sensitive?
     * @return Return total number of items removed
     */
    static size_t removeAllThatDoNotEndWith(std::vector<std::string> &input_strings,
                                            const std::string &input_string_to_remove,
                                            bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);



    /**
     * @brief removeAllThatEndWith Removes all items from input_strings that
     * end with input_string_to_remove
     * @param input_strings Reference to vector of strings
     * @param input_string_to_remove The string to remove
     * @param is_case_sensitive Should check be case sensitive?
     * @return Return total number of items removed
     */
    static size_t removeAllThatEndWith(std::vector<std::string> &input_strings,
                                const std::string &input_string_to_remove,
                                bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);


    /**
     * @brief removeAllThatStartWith Removes all items from input_strings that
     * start with input_string_to_remove
     * @param input_strings Reference to vector of strings
     * @param input_string_to_remove The string to remove
     * @param is_case_sensitive Should check be case sensitive?
     * @return Return total number of items removed
     */
    static size_t removeAllThatStartWith(std::vector<std::string> &input_strings,
                                const std::string &input_string_to_remove,
                                bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);


    /**
     * @brief removeAllThatDoNotStartWith Removes all items from input_strings that
     * DO NOT start with input_string_to_remove
     * @param input_strings Reference to vector of strings
     * @param input_string_to_remove The string to remove
     * @param is_case_sensitive Should check be case sensitive?
     * @return Return total number of items removed
     */
    static size_t removeAllThatDoNotStartWith(std::vector<std::string> &input_strings,
                                              const std::string &input_string_to_remove,
                                              bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

    /**
     * @brief removeAllThatContain Removes all items from input_strings that
     * contain input_string_to_remove
     * @param input_strings Reference to vector of strings
     * @param input_string_to_remove The string to remove
     * @param is_case_sensitive Should check be case sensitive?
     * @return Return total number of items removed
     */
    static size_t removeAllThatContain(std::vector<std::string> &input_strings,
                                const std::string &input_string_to_remove,
                                bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);

    /**
     * @brief removeAllThatDoNotContain Removes all items from vector that do not contain the input_string
     * @param input_strings Reference to vector to remove items from
     * @param input_string The input string to search for
     * @param is_case_sensitive Should comparision be case sensitive?
     * @return Return total number of items removed
     */
    static size_t removeAllThatDoNotContain(std::vector<std::string> &input_strings,
                                            const std::string &input_string,
                                            bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);
    /**
     * @brief removeNonAlphaNumeric Removes alphanumeric characters from input_string
     * @param input_string The string to process
     */
    static void removeNonAlphaNumericInPlace(std::string & input_string);

    /**
     * @brief removeNonAlphaNumeric (Convience Function) Removes alphanumeric characters from input_string
     * @param input_string The string to process
     * @return Return new string with no alphanumeric characters.
     */
    static std::string removeNonAlphaNumeric(const std::string &input_string);

	/**
     * @brief splitKeyValue Splits string into two parts based on string token.
     * @param input_string The input string to process
     * @param delimiter_str The delimiter to split on
     * @param trim_items Should items be trimmed after split? (defaults to true)
     * @return Return split string if delimiter_str exists in input_string, otherwise an
     *         empty pair<string,string> is returned
     * @code std::string example = " T1<%>  T2";
     *       auto item = splitKeyValue(example,"<%>");
     *       //item.first = "T1"
     *       //item.second = "T2"
	 */
    static std::pair<std::string,std::string> splitKeyValue(const std::string& input_string,
                                                            const std::string& delimiter_str,
                                                            bool trim_items = true);

	/**
     * @brief isEmpty Checks to see if string is empty.  Trims whitespace before checking.
     * @param input_string The string to check
     * @return Return true if string is empty, false otherwise
	 */
    static bool isEmpty(const std::string& input_string);

	/**
     * @brief isNumeric Determines if input string is a number
     * @param input_string The input string
     * @param number_base The numeric base (8,10 or 16)
     * @return Return true if input_string is numeric, false otherwise
	 */
    static bool isNumeric(const std::string &input_string, size_t number_base = 10);

    /**
     * @brief createUniqueUUIDList Generate a list of unique UUIDs (GUIDs)
     * @param count The number of UUIDs to generate
     * @param include_dashes Whether or not to include dashes in UUID
     * @return Return vector containing list of unique UUIDs
     */
    static std::vector<std::string> createUniqueUUIDList(size_t count,
                                                         bool include_dashes = false);

    /**
     * @brief removeTextAfter Removes text after a certain string
     * @param input_string The string to process
     * @param text_after_delimiter_str The string to search for
     * @param keep_delimiter Determines whether or not the text_after_delimiter_str
     * should be included in results
     * @return Return the text that occurs after "text_after_str". If text_after_str
     * is not found then an empty string is returned
     */
    static std::string removeTextAfter(const std::string& input_string,
                                       const std::string& text_after_delimiter_str,
                                       bool keep_delimiter = false);

    /**
     * @brief multiequals Returns true if any string in vector matches input_string
     * @param input_string The string to compare against
     * @param comparisons Vector of comparision strings
     * @return Return true if match exists, otherwise return false
     */
    static bool multiequals(const std::string& input_string,
                            const std::vector<std::string> &comparisons,
                            bool is_case_sensitive = c_DEFAULT_CASE_SENSITIVE);
};

template <typename T>
std::string StringUtils::toHexString(T value)
{
    std::ostringstream out;
    out << std::hex << value;
    return "0x" + toLower(out.str());
}

template <typename T>
std::string StringUtils::toBitString(T input_value)
{
    return std::bitset<64>(input_value).to_string(); // string conversion
}

template<typename T>
inline T StringUtils::toNumber(const std::string &input_value)
{
    std::istringstream in(input_value,std::istringstream::in);
    T returnValue;
    in>>returnValue;
    return returnValue;
}

template <typename T>
void StringUtils::printVector(std::ostream & out,
                                 const std::vector<T> & values)
{
    size_t count = values.size();
    for (size_t a1 = 0;a1<count;++a1)
    {
        out <<"["<<a1<<"] = "<< values[a1]<<std::endl;
    }
}

template <typename T>
std::string StringUtils::toString(T value)
{
    return std::to_string(value);
}
