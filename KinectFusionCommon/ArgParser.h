/***********************************************************/
/**	\file
	\brief		Arg parser
	\author		Yizhong Zhang
	\date		7/6/2019
*/
/***********************************************************/
#ifndef YZ_ARG_PARSER_H
#define YZ_ARG_PARSER_H

#pragma warning(disable:4996)

/**
example code to parse a format string:

va_list args;
va_start(args, format);
int len = _vscprintf(format, args) + 1;
char* buffer = new char[len];
vsprintf(buffer, format, args);
va_end(args);
*/
class ArgParser {
public:
	/**
	utility function: seperate buffer into cmd. buffer will be changed

	line seperator: '|', '\n'
	word seperator: ' ', '\t'

	example input:	buffer = "rgbx8 %p | depth16 %p"
	example output:	cmd[0] = "rgbx8", cmd[1] = "%p", cmd[2] = "depth16", cmd[3] = "%p"
	example return: 2

	\param	cmd			the seperated cmd, each line occupy 2 cmd
	\param	cmd_size	size of cmd array
	\param	buffer		the input cmd string
	\return				number of lines
	*/
	int SeperateCmd(char* cmd[], int cmd_size, char* buffer) {
		if (!buffer)
			return 0;

		//	create a static delimiter table
		static char delimiters[256];	//	0 bit: line seperator, 1 bit: word seperator
		static int delimiters_init_flag = 0;
		if (!delimiters_init_flag) {
			for (int i = 0; i < 256; i++)
				delimiters[i] = 0;
			delimiters[int('|')] |= 0x01;
			delimiters[int('\n')] |= 0x01;
			delimiters[int(' ')] |= 0x02;
			delimiters[int('\t')] |= 0x02;
		}

		//	each command occupy 2 lines in cmd, so cmd_size must be even
		cmd_size /= 2;
		cmd_size *= 2;

		//	parse the buffer string
		int line_counter = 0;
		char* ptr = buffer;

		while (*ptr) {
			//	skip empty lines or whitespace
			while (*ptr && (delimiters[*ptr] & 0x03)) {	//	line or word seperator
				*ptr = '\0';
				ptr++;
			}
			if (!(*ptr))	//	no word any more
				break;

			//	we are at position of this cmd, if no more space, just ignore the following
			if (line_counter * 2 >= cmd_size) {
				std::cout << "SeperateCmd max_lines: " << cmd_size << ", ignore the following cmd: " << ptr << std::endl;
				break;
			}

			//	extract command (first word of the line)
			cmd[line_counter * 2] = ptr;
			while (*ptr && !(delimiters[*ptr] & 0x03))
				ptr++;
			if (!(*ptr)) {	//	this line only contain one word, and has reached the end of buffer, break
				cmd[line_counter * 2 + 1] = ptr;
				line_counter++;
				break;
			}
			if (delimiters[*ptr] & 0x01) {	//	this line only contain cmd, begin another line
				*ptr = '\0';
				cmd[line_counter * 2 + 1] = ptr;
				line_counter++;
				ptr++;
				continue;
			}

			//	first word already extracted, skip empty whitespace and extract following cmds
			while (*ptr && (delimiters[*ptr] & 0x02)) {
				*ptr = '\0';
				ptr++;
			}
			if (!(*ptr)) {					//	no following cmd any more
				cmd[line_counter * 2 + 1] = ptr;
				line_counter++;
				break;
			}
			if (delimiters[*ptr] & 0x01) {	//	no more cmd in this line
				*ptr = '\0';
				cmd[line_counter * 2 + 1] = ptr;
				line_counter++;
				ptr++;
				break;
			}

			//	record cmds
			cmd[line_counter * 2 + 1] = ptr;
			line_counter++;

			//	goto the end of current cmd
			while (*ptr && !(delimiters[*ptr] & 0x01))
				ptr++;

			//	remove the back empty space
			char* inv_ptr = ptr - 1;
			while (*inv_ptr && (delimiters[*inv_ptr] & 0x02)) {
				(*inv_ptr) = '\0';
				inv_ptr--;
			}
		}

		return line_counter;
	}
};


#endif	//	YZ_ARG_PARSER_H