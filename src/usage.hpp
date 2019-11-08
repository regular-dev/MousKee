/*
   Copyleft (ɔ) 2009 Kernc
   This program is free software. It comes with absolutely no warranty whatsoever.
   See COPYING for further information.

   Code used from : https://github.com/kernc/logkeys
 */

#ifndef _USAGE_H_
#define _USAGE_H_

void usage()
{
		fprintf(stderr,
		        "Usage: mouskee [OPTION]...\n"
		        "Handle mouse pointer with keyboard arrows\n"
		        "\n"
		        "  -s, --start               start\n"
		        "  -m, --keymap=FILE         use keymap FILE\n"
		        "  -u, --us-keymap           use en_US keymap instead of configured default\n"
		        "  -k, --kill                kill running logkeys process\n"
		        "  -d, --device=FILE         input event device [eventX from " INPUT_EVENT_PATH "]\n"
		        "  -?, --help                print this help screen\n"
		        "      --export-keymap=FILE  export configured keymap to FILE and exit\n"
		        "      --no-func-keys        log only character keys\n"
		        "      --post-http=URL       POST log to URL as multipart/form-data file\n"
		        "      --no-daemon           run in foreground\n"
		        "\n"
		        "Examples: sudo ./mouskee -s -d event5 --no-daemon \n"
		        "mousekee version: " PACKAGE_VERSION "\n"
		        "Code used from unix keylogger : <https://github.com/kernc/logkeys/>\n"
						"MouseKee homepage : <regular-dev.org/mouskee>"
		        );
}

#endif  // _USAGE_H_
