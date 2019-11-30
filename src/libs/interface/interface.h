#ifndef INTERFACE_H
#define INTERFACE_H

void init_interface(int argc, char *argv[]);



/* selection_changed
 * Description:
 *      - Function called for all modification of image. It will change the image on the screen by the the selected one.
 * */
void home_selection_changed();


/* gtk_get_file_to_save
 * Input:
 *      - filename (char **): Address to a char, to save the path
 *
 * Return;
 *      - int (the state of the function, 1 if we choose a place, 0 else)
 *
 *  Description:
 *      - Open a file chooser to find a place to save the created file.
 * */
int gtk_get_file_to_save(char **filename);


/* result_btn_cancel_clicked
 * Description:
      - Function called when an user doesn't want to save the output
 * * */
void result_btn_cancel_clicked();


/* result_btn_save_clicked
 * Description:
      - Function called when an user wants to save the output. Open an file chooser and save the file
 * * */
void result_btn_save_clicked();


/* btn_run_clicked
 * Description:
      - Function called when an user wants to start the process
 * * */
void btn_run_clicked();




#endif