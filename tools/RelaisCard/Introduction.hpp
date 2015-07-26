// This file contains doxygen annotations only to generate the introduction page

/*! \mainpage My Personal Index Page
 *
 * \section sec_intro Introduction
 *
 * The purpose of Relaiscardd and Relaiscardd is to control
 * an 8 port relais card offered by Conrad Electronic:
 * http://www.conrad.de/ce/de/product/197720/CE-8fach-Relaiskarte-Baustein-12-24-VDC-8-Relaisausgaenge
 * from command line
 *
 * \subsection sub_Relaiscardd Relaiscardd
 * Relaiscardd is running in the background as deamon and communicates with the relais card.
 * It's purpose is to queue up incoming requests in case of multiple access to
 * the relais card.
 *
 * \subsection sub_Relaiscard Relaiscard
 * Relaiscard needs to be executed everytime the state of a relais needs to be changed.
 * Relaiscard forwards the command to Relaiscardd daemon which forwards the command
 * to the hardware
 */
