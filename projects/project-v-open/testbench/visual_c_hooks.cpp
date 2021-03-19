/////////////////////////////////////////////
//
// Last update by Liran Xiao, 09/2019
//
/////////////////////////////////////////////
#include "DirectC.h"
#include <curses.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <time.h>
#include <string.h>

#include "riscv_inst.h"

#define PARENT_READ     readpipe[0]
#define CHILD_WRITE     readpipe[1]
#define CHILD_READ      writepipe[0]
#define PARENT_WRITE    writepipe[1]
#define NUM_HISTORY     256
#define NUM_ARF         32
#define NUM_STAGES      5
#define NOOP_INST       0x00000013
#define NUM_REG_GROUPS  4
#define REG_SIZE_IN_HEX 8

// random variables/stuff
int fd[2], writepipe[2], readpipe[2];
int stdout_save;
int stdout_open;
void signal_handler_IO (int status);
int wait_flag=0;
char done_state;
char echo_data;
FILE *fp;
FILE *fp2;
int setup_registers = 0;
int stop_time;
int done_time = -1;
char time_wrapped = 0;

// Structs to hold information about each register/signal group
typedef struct win_info {
  int height;
  int width;
  int starty;
  int startx;
  int color;
} win_info_t;

typedef struct reg_group {
  WINDOW *reg_win;
  char ***reg_contents;
  char **reg_names;
  int num_regs;
  win_info_t reg_win_info;
} reg_group_t;

// Window pointers for ncurses windows
WINDOW *title_win;
WINDOW *comment_win;
WINDOW *time_win;
WINDOW *sim_time_win;
WINDOW *instr_win;
WINDOW *clock_win;
WINDOW *pipe_win;
WINDOW *if_win;
WINDOW *if_id_win;
WINDOW *id_win;
WINDOW *id_ex_win;
WINDOW *ex_win;
WINDOW *ex_mem_win;
WINDOW *mem_win;
WINDOW *mem_wb_win;
WINDOW *wb_win;
WINDOW *arf_win;
WINDOW *misc_win;
WINDOW *vtuber_win;

// arrays for register contents and names
int history_num=0;
int num_misc_regs;
int num_if_regs = 0;
int num_if_id_regs = 0;
int num_id_regs = 0;
int num_id_ex_regs = 0;
int num_ex_regs = 0;
int num_ex_mem_regs = 0;
int num_mem_regs = 0;
int num_mem_wb_regs = 0;
int num_wb_regs = 0;
char readbuffer[1024];
char **timebuffer;
char **cycles;
char *clocks;
char *resets;
char **inst_contents;
char ***if_contents;
char ***if_id_contents;
char ***id_contents;
char ***id_ex_contents;
char ***ex_contents;
char ***ex_mem_contents;
char ***mem_contents;
char ***mem_wb_contents;
char ***wb_contents;
char **arf_contents;
char ***misc_contents;
char **if_reg_names;
char **if_id_reg_names;
char **id_reg_names;
char **id_ex_reg_names;
char **ex_reg_names;
char **ex_mem_reg_names;
char **mem_reg_names;
char **mem_wb_reg_names;
char **wb_reg_names;
char **misc_reg_names;

char *get_opcode_str(int inst, int valid_inst);
void parse_register(char* readbuf, int reg_num, char*** contents, char** reg_names);
int get_time();


// Helper function for ncurses gui setup
WINDOW *create_newwin(int height, int width, int starty, int startx, int color){
  WINDOW *local_win;
  local_win = newwin(height, width, starty, startx);
  wbkgd(local_win,COLOR_PAIR(color));
  wattron(local_win,COLOR_PAIR(color));
  box(local_win,0,0);
  wrefresh(local_win);
  return local_win;
}

// Function to draw positive edge or negative edge in clock window
void update_clock(char clock_val){
  static char cur_clock_val = 0;
  // Adding extra check on cycles because:
  //  - if the user, right at the beginning of the simulation, jumps to a new
  //    time right after a negative clock edge, the clock won't be drawn
  if((clock_val != cur_clock_val) || strncmp(cycles[history_num],"      0",7) == 1){
    mvwaddch(clock_win,3,7,ACS_VLINE | A_BOLD);
    if(clock_val == 1){

      //we have a posedge
      mvwaddch(clock_win,2,1,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,ACS_ULCORNER | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      mvwaddch(clock_win,4,1,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_LRCORNER | A_BOLD);
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
    } else {

      //we have a negedge
      mvwaddch(clock_win,4,1,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,ACS_LLCORNER | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      mvwaddch(clock_win,2,1,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_HLINE | A_BOLD);
      waddch(clock_win,ACS_URCORNER | A_BOLD);
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
      waddch(clock_win,' ');
    }
  }
  cur_clock_val = clock_val;
  wrefresh(clock_win);
}

// Function to create and initialize the gui
// Color pairs are (foreground color, background color)
// If you don't like the dark backgrounds, a safe bet is to have
//   COLOR_BLUE/BLACK foreground and COLOR_WHITE background
void setup_gui(FILE *fp){
  initscr();
  if(has_colors()){
    start_color();
    init_pair(1,COLOR_CYAN,COLOR_BLACK);    // shell background
    init_pair(2,COLOR_YELLOW,COLOR_RED);
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLUE);   // title window
    init_pair(5,COLOR_YELLOW,COLOR_BLACK);  // register/signal windows
    init_pair(6,COLOR_RED,COLOR_BLACK);
    init_pair(7,COLOR_MAGENTA,COLOR_BLACK); // pipeline window
    init_pair(8,COLOR_BLUE, COLOR_BLACK);
  }
  curs_set(0);
  noecho();
  cbreak();
  keypad(stdscr,TRUE);
  wbkgd(stdscr,COLOR_PAIR(1));
  wrefresh(stdscr);
  int pipe_width=0;

  //instantiate the title window at top of screen
  title_win = create_newwin(3,COLS,0,0,4);
  mvwprintw(title_win,1,1,"SIMULATION INTERFACE V1");
  mvwprintw(title_win,1,COLS-22,"BEN KEMPKE/JOSH SMITH");
  wrefresh(title_win);

  //instantiate time window at right hand side of screen
  time_win = create_newwin(3,10,8,COLS-10,5);
  mvwprintw(time_win,0,3,"TIME");
  wrefresh(time_win);

  //instantiate a sim time window which states the actual simlator time
  sim_time_win = create_newwin(3,10,11,COLS-10,5);
  mvwprintw(sim_time_win,0,1,"SIM TIME");
  wrefresh(sim_time_win);

  //instantiate a window to show which clock edge this is
  clock_win = create_newwin(6,15,8,COLS-25,5);
  mvwprintw(clock_win,0,5,"CLOCK");
  mvwprintw(clock_win,1,1,"cycle:");
  update_clock(0);
  wrefresh(clock_win);

  // instantiate a window for the ARF on the right side
  arf_win = create_newwin(34,25,14,COLS-25,5);
  mvwprintw(arf_win,0,13,"ARF");
  int i=0;
  char tmp_buf[32];
  for (; i < NUM_ARF; i++) {
    sprintf(tmp_buf, "x%02d: ", i);
    mvwprintw(arf_win,i+1,1,tmp_buf);
  }
  wrefresh(arf_win);

  //instantiate window to visualize instructions in pipeline below title
  pipe_win = create_newwin(5,COLS,3,0,7);
  pipe_width = COLS/6;
  mvwprintw(pipe_win,0,(COLS-8)/2,"PIPELINE");
  wattron(pipe_win,A_UNDERLINE);
  mvwprintw(pipe_win,1,pipe_width-2,"IF");
  mvwprintw(pipe_win,1,2*pipe_width-2,"ID");
  mvwprintw(pipe_win,1,3*pipe_width-2,"EX");
  mvwprintw(pipe_win,1,4*pipe_width-3,"MEM");
  mvwprintw(pipe_win,1,5*pipe_width-3,"WB");
  wattroff(pipe_win,A_UNDERLINE);
  wrefresh(pipe_win);

  //instantiate window to visualize IF stage (including IF/ID)
  if_win = create_newwin((num_if_regs+2),30,8,0,5);
  mvwprintw(if_win,0,10,"IF STAGE");
  wrefresh(if_win);

  //instantiate window to visualize IF/ID signals
  if_id_win = create_newwin((num_if_id_regs+2),30,8+(num_if_regs+2),0,5);
  mvwprintw(if_id_win,0,12,"IF/ID");
  wrefresh(if_id_win);

  //instantiate a window to visualize ID stage
  id_win = create_newwin((num_id_regs+2),30,8,30,5);
  mvwprintw(id_win,0,10,"ID STAGE");
  wrefresh(id_win);

  //instantiate a window to visualize ID/EX signals
  id_ex_win = create_newwin((num_id_ex_regs+2),30,8,60,5);
  mvwprintw(id_ex_win,0,12,"ID/EX");
  wrefresh(id_ex_win);

  //instantiate a window to visualize EX stage
  ex_win = create_newwin((num_ex_regs+2),30,8,90,5);
  mvwprintw(ex_win,0,10,"EX STAGE");
  wrefresh(ex_win);

  //instantiate a window to visualize EX/MEM
  ex_mem_win = create_newwin((num_ex_mem_regs+2),30,LINES-7-(num_ex_mem_regs+2),0,5);
  mvwprintw(ex_mem_win,0,12,"EX/MEM");
  wrefresh(ex_mem_win);

  //instantiate a window to visualize MEM stage
  mem_win = create_newwin((num_mem_regs+2),30,LINES-7-(num_mem_regs+2),30,5);
  mvwprintw(mem_win,0,10,"MEM STAGE");
  wrefresh(mem_win);

  //instantiate a window to visualize MEM/WB
  mem_wb_win = create_newwin((num_mem_wb_regs+2),30,LINES-7-(num_mem_wb_regs+2),60,5);
  mvwprintw(mem_wb_win,0,12,"MEM/WB");
  wrefresh(mem_wb_win);


  //instantiate a window to visualize WB stage
  wb_win = create_newwin((num_wb_regs+2),30,LINES-7-(num_wb_regs+2),90,5);
  mvwprintw(wb_win,0,10,"WB STAGE");
  wrefresh(wb_win);

  //instantiate an instructional window to help out the user some
  instr_win = create_newwin(7,30,LINES-7,0,5);
  mvwprintw(instr_win,0,9,"INSTRUCTIONS");
  wattron(instr_win,COLOR_PAIR(5));
  mvwaddstr(instr_win,1,1,"'n'   -> Next clock edge");
  mvwaddstr(instr_win,2,1,"'b'   -> Previous clock edge");
  mvwaddstr(instr_win,3,1,"'c/g' -> Goto specified time");
  mvwaddstr(instr_win,4,1,"'r'   -> Run to end of sim");
  mvwaddstr(instr_win,5,1,"'q'   -> Quit Simulator");
  wrefresh(instr_win);
  
  // instantiate window to visualize misc regs/wires
  misc_win = create_newwin(7,25,LINES-7,30,5);
  mvwprintw(misc_win,0,(COLS-30-30)/2-6,"MISC SIGNALS");
  wrefresh(misc_win);

  vtuber_win = create_newwin(10, 47, LINES-10, COLS-47, 8);
  mvwaddstr(vtuber_win, 2, 4, "__     _______ _   _ ____  _____ ____");
  mvwaddstr(vtuber_win, 3, 4, "\\ \\   / /_   _| | | | __ )| ____|  _ \\");
  mvwaddstr(vtuber_win, 4, 4, " \\ \\ / /  | | | | | |  _ \\|  _| | |_) |");
  mvwaddstr(vtuber_win, 5, 4, "  \\ V /   | | | |_| | |_) | |___|  _ <");
  mvwaddstr(vtuber_win, 6, 4, "   \\_/    |_|  \\___/|____/|_____|_| \\_\\");
  wrefresh(vtuber_win);

  refresh();
}

// This function updates all of the signals being displayed with the values
// from time history_num_in (this is the index into all of the data arrays).
// If the value changed from what was previously display, the signal has its
// display color inverted to make it pop out.
void parsedata(int history_num_in){
  static int old_history_num_in=0;
  static int old_head_position=0;
  static int old_tail_position=0;
  int i=0;
  int data_counter=0;
  char *opcode;
  int tmp=0;
  int tmp_val=0;
  char tmp_buf[32];
  int pipe_width = COLS/6;

  // Handle updating resets
  if (resets[history_num_in]) {
    wattron(title_win,A_REVERSE);
    mvwprintw(title_win,1,(COLS/2)-3,"RESET");
    wattroff(title_win,A_REVERSE);
  }
  else if (done_time != 0 && (history_num_in == done_time)) {
    wattron(title_win,A_REVERSE);
    mvwprintw(title_win,1,(COLS/2)-3,"DONE ");
    wattroff(title_win,A_REVERSE);
  }
  else
    mvwprintw(title_win,1,(COLS/2)-3,"     ");
  wrefresh(title_win);

  // Handle updating the pipeline window
  for(i=0; i < NUM_STAGES; i++) {
    strncpy(tmp_buf,inst_contents[history_num_in]+i*9,8);
    tmp_buf[9] = '\0';
    sscanf(tmp_buf,"%8x", &tmp_val);
    tmp = (int)inst_contents[history_num_in][8+(i*9)] - (int)'0';
    opcode = get_opcode_str(tmp_val, tmp);

    // clear string and overwrite
    mvwprintw(pipe_win,2,pipe_width*(i+1)-2-5,"          ");
    if (strncmp(tmp_buf,"xxxxxxxx",8) == 0)
      mvwaddnstr(pipe_win,2,pipe_width*(i+1)-2-4,tmp_buf,8);
    else
      mvwaddstr(pipe_win,2,pipe_width*(i+1)-2-(strlen(opcode)/2),opcode);
    if (tmp==0 || tmp==((int)'x'-(int)'0'))
      mvwprintw(pipe_win,3,pipe_width*(i+1)-2,"I");
    else
      mvwprintw(pipe_win,3,pipe_width*(i+1)-2,"V");

  }
  wrefresh(pipe_win);

  // Handle updating the ARF window
  int num_size = REG_SIZE_IN_HEX;
  for (i=0; i < NUM_ARF; i++) {
    if (strncmp(arf_contents[history_num_in]+i*num_size,
                arf_contents[old_history_num_in]+i*num_size,num_size))
      wattron(arf_win, A_REVERSE);
    else
      wattroff(arf_win, A_REVERSE);
    mvwaddnstr(arf_win,i+1,6,arf_contents[history_num_in]+i*num_size,num_size);
  }
  wrefresh(arf_win);


  // Handle updating the IF window
  for(i=0;i<num_if_regs;i++){
    if (strcmp(if_contents[history_num_in][i],
                if_contents[old_history_num_in][i]))
      wattron(if_win, A_REVERSE);
    else
      wattroff(if_win, A_REVERSE);
    mvwaddstr(if_win,i+1,strlen(if_reg_names[i])+3,if_contents[history_num_in][i]);
  }
  wrefresh(if_win);

  // Handle updating the IF/ID window
  for(i=0;i<num_if_id_regs;i++){
    if (strcmp(if_id_contents[history_num_in][i],
                if_id_contents[old_history_num_in][i]))
      wattron(if_id_win, A_REVERSE);
    else
      wattroff(if_id_win, A_REVERSE);
    mvwaddstr(if_id_win,i+1,strlen(if_id_reg_names[i])+3,if_id_contents[history_num_in][i]);
  }
  wrefresh(if_id_win);

  // Handle updating the ID window
  for(i=0;i<num_id_regs;i++){
    if (strcmp(id_contents[history_num_in][i],
                id_contents[old_history_num_in][i]))
      wattron(id_win, A_REVERSE);
    else
      wattroff(id_win, A_REVERSE);
    mvwaddstr(id_win,i+1,strlen(id_reg_names[i])+3,id_contents[history_num_in][i]);
  }
  wrefresh(id_win);


  // Handle updating the ID/EX window
  for(i=0;i<num_id_ex_regs;i++){
    if (strcmp(id_ex_contents[history_num_in][i],
                id_ex_contents[old_history_num_in][i]))
      wattron(id_ex_win, A_REVERSE);
    else
      wattroff(id_ex_win, A_REVERSE);
    mvwaddstr(id_ex_win,i+1,strlen(id_ex_reg_names[i])+3,id_ex_contents[history_num_in][i]);
  }
  wrefresh(id_ex_win);

  // Handle updating the EX window
  for(i=0;i<num_ex_regs;i++){
    if (strcmp(ex_contents[history_num_in][i],
                ex_contents[old_history_num_in][i]))
      wattron(ex_win, A_REVERSE);
    else
      wattroff(ex_win, A_REVERSE);
    mvwaddstr(ex_win,i+1,strlen(ex_reg_names[i])+3,ex_contents[history_num_in][i]);
  }
  wrefresh(ex_win);

  // Handle updating the EX/MEM window
  for(i=0;i<num_ex_mem_regs;i++){
    if (strcmp(ex_mem_contents[history_num_in][i],
                ex_mem_contents[old_history_num_in][i]))
      wattron(ex_mem_win, A_REVERSE);
    else
      wattroff(ex_mem_win, A_REVERSE);
    mvwaddstr(ex_mem_win,i+1,strlen(ex_mem_reg_names[i])+3,ex_mem_contents[history_num_in][i]);
  }
  wrefresh(ex_mem_win);

  // Handle updating the MEM window
  for(i=0;i<num_mem_regs;i++){
    if (strcmp(mem_contents[history_num_in][i],
                mem_contents[old_history_num_in][i]))
      wattron(mem_win, A_REVERSE);
    else
      wattroff(mem_win, A_REVERSE);
    mvwaddstr(mem_win,i+1,strlen(mem_reg_names[i])+3,mem_contents[history_num_in][i]);
  }
  wrefresh(mem_win);

  // Handle updating the MEM/WB window
  for(i=0;i<num_mem_wb_regs;i++){
    if (strcmp(mem_wb_contents[history_num_in][i],
                mem_wb_contents[old_history_num_in][i]))
      wattron(mem_wb_win, A_REVERSE);
    else
      wattroff(mem_wb_win, A_REVERSE);
    mvwaddstr(mem_wb_win,i+1,strlen(mem_wb_reg_names[i])+3,mem_wb_contents[history_num_in][i]);
  }
  wrefresh(mem_wb_win);

  // Handle updating the WB window
  for(i=0;i<num_wb_regs;i++){
    if (strcmp(wb_contents[history_num_in][i],
                wb_contents[old_history_num_in][i]))
      wattron(wb_win, A_REVERSE);
    else
      wattroff(wb_win, A_REVERSE);
    mvwaddstr(wb_win,i+1,strlen(wb_reg_names[i])+3,wb_contents[history_num_in][i]);
  }
  wrefresh(wb_win);

  // Handle updating the misc. window
  int row=1,col=1;
  for (i=0;i<num_misc_regs;i++){
    if (strcmp(misc_contents[history_num_in][i],
                misc_contents[old_history_num_in][i]))
      wattron(misc_win, A_REVERSE);
    else
      wattroff(misc_win, A_REVERSE);
    

    mvwaddstr(misc_win,(i%5)+1,((i/5)*30)+strlen(misc_reg_names[i])+3,misc_contents[history_num_in][i]);
    if ((++row)>6) {
      row=1;
      col+=30;
    }
  }
  wrefresh(misc_win);

  //update the time window
  mvwaddstr(time_win,1,1,timebuffer[history_num_in]);
  wrefresh(time_win);

  //update to the correct clock edge for this history
  mvwaddstr(clock_win,1,7,cycles[history_num_in]);
  update_clock(clocks[history_num_in]);

  //save the old history index to check for changes later
  old_history_num_in = history_num_in;
}

// Parse a line of data output from the testbench
int processinput(){
  static int byte_num = 0;
  static int if_reg_num = 0;
  static int if_id_reg_num = 0;
  static int id_reg_num = 0;
  static int id_ex_reg_num = 0;
  static int ex_reg_num = 0;
  static int ex_mem_reg_num = 0;
  static int mem_reg_num = 0;
  static int mem_wb_reg_num = 0;
  static int wb_reg_num = 0;
  static int misc_reg_num = 0;
  int tmp_len;
  char name_buf[32];
  char val_buf[32];

  //get rid of newline character
  readbuffer[strlen(readbuffer)-1] = 0;

  if(strncmp(readbuffer,"t",1) == 0){

    //We are getting the timestamp
    strcpy(timebuffer[history_num],readbuffer+1);
  }else if(strncmp(readbuffer,"c",1) == 0){

    //We have a clock edge/cycle count signal
    if(strncmp(readbuffer+1,"0",1) == 0)
      clocks[history_num] = 0;
    else
      clocks[history_num] = 1;

    // grab clock count (for some reason, first clock count sent is
    // too many digits, so check for this)
    strncpy(cycles[history_num],readbuffer+2,7);
    if (strncmp(cycles[history_num],"       ",7) == 0)
      cycles[history_num][6] = '0';
    
  }else if(strncmp(readbuffer,"z",1) == 0){
    
    // we have a reset signal
    if(strncmp(readbuffer+1,"0",1) == 0)
      resets[history_num] = 0;
    else
      resets[history_num] = 1;

  }else if(strncmp(readbuffer,"a",1) == 0){
    // We are getting ARF registers
    strcpy(arf_contents[history_num], readbuffer+1);

  }else if(strncmp(readbuffer,"p",1) == 0){
    // We are getting information about which instructions are in each stage
    strcpy(inst_contents[history_num], readbuffer+1);

  }else if(strncmp(readbuffer,"f",1) == 0){
    // We are getting an IF register

    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, if_reg_num, if_contents, if_reg_names);
      mvwaddstr(if_win,if_reg_num+1,1,if_reg_names[if_reg_num]);
      waddstr(if_win, ": ");
      wrefresh(if_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(if_contents[history_num][if_reg_num],val_buf);
    }

    if_reg_num++;
  }else if(strncmp(readbuffer,"g",1) == 0){
    // We are getting an IF/ID register

    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, if_id_reg_num, if_id_contents, if_id_reg_names);
      mvwaddstr(if_id_win,if_id_reg_num+1,1,if_id_reg_names[if_id_reg_num]);
      waddstr(if_id_win, ": ");
      wrefresh(if_id_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(if_id_contents[history_num][if_id_reg_num],val_buf);
    }

    if_id_reg_num++;
  }else if(strncmp(readbuffer,"d",1) == 0){
    // We are getting an ID register
    
    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, id_reg_num, id_contents, id_reg_names);
      mvwaddstr(id_win,id_reg_num+1,1,id_reg_names[id_reg_num]);
      waddstr(id_win, ": ");
      wrefresh(id_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(id_contents[history_num][id_reg_num],val_buf);
    }

    id_reg_num++;
  }else if(strncmp(readbuffer,"h",1) == 0){
    // We are getting an ID/EX register

    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, id_ex_reg_num, id_ex_contents, id_ex_reg_names);
      mvwaddstr(id_ex_win,id_ex_reg_num+1,1,id_ex_reg_names[id_ex_reg_num]);
      waddstr(id_ex_win, ": ");
      wrefresh(id_ex_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(id_ex_contents[history_num][id_ex_reg_num],val_buf);
    }

    id_ex_reg_num++;
  }else if(strncmp(readbuffer,"e",1) == 0){
    // We are getting an EX register

    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, ex_reg_num, ex_contents, ex_reg_names);
      mvwaddstr(ex_win,ex_reg_num+1,1,ex_reg_names[ex_reg_num]);
      waddstr(ex_win, ": ");
      wrefresh(ex_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(ex_contents[history_num][ex_reg_num],val_buf);
    }

    ex_reg_num++;
  }else if(strncmp(readbuffer,"i",1) == 0){
    // We are getting an EX/MEM register

    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, ex_mem_reg_num, ex_mem_contents, ex_mem_reg_names);
      mvwaddstr(ex_mem_win,ex_mem_reg_num+1,1,ex_mem_reg_names[ex_mem_reg_num]);
      waddstr(ex_mem_win, ": ");
      wrefresh(ex_mem_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(ex_mem_contents[history_num][ex_mem_reg_num],val_buf);
    }

    ex_mem_reg_num++;
  }else if(strncmp(readbuffer,"m",1) == 0){
    // We are getting a MEM register

    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, mem_reg_num, mem_contents, mem_reg_names);
      mvwaddstr(mem_win,mem_reg_num+1,1,mem_reg_names[mem_reg_num]);
      waddstr(mem_win, ": ");
      wrefresh(mem_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(mem_contents[history_num][mem_reg_num],val_buf);
    }

    mem_reg_num++;
  }else if(strncmp(readbuffer,"j",1) == 0){
    // We are getting an MEM/WB register

    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, mem_wb_reg_num, mem_wb_contents, mem_wb_reg_names);
      mvwaddstr(mem_wb_win,mem_wb_reg_num+1,1,mem_wb_reg_names[mem_wb_reg_num]);
      waddstr(mem_wb_win, ": ");
      wrefresh(mem_wb_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(mem_wb_contents[history_num][mem_wb_reg_num],val_buf);
    }

    mem_wb_reg_num++;
  }else if(strncmp(readbuffer,"w",1) == 0){
    // We are getting a WB register

    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, wb_reg_num, wb_contents, wb_reg_names);
      mvwaddstr(wb_win,wb_reg_num+1,1,wb_reg_names[wb_reg_num]);
      waddstr(wb_win, ": ");
      wrefresh(wb_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(wb_contents[history_num][wb_reg_num],val_buf);
    }

    wb_reg_num++;
  }else if(strncmp(readbuffer,"v",1) == 0){

    //we are processing misc register/wire data
    // If this is the first time we've seen the register,
    // add name and data to arrays
    if (!setup_registers) {
      parse_register(readbuffer, misc_reg_num, misc_contents, misc_reg_names);
      mvwaddstr(misc_win,(misc_reg_num%5)+1,(misc_reg_num/5)*30+1,misc_reg_names[misc_reg_num]);
      waddstr(misc_win, ": ");
      wrefresh(misc_win);
    } else {
      sscanf(readbuffer,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
      strcpy(misc_contents[history_num][misc_reg_num],val_buf);
    }

    misc_reg_num++;
  }else if (strncmp(readbuffer,"break",4) == 0) {
    // If this is the first time through, indicate that we've setup all of
    // the register arrays.
    setup_registers = 1;

    //we've received our last data segment, now go process it
    byte_num = 0;
    if_reg_num = 0;
    if_id_reg_num = 0;
    id_reg_num = 0;
    id_ex_reg_num = 0;
    ex_reg_num = 0;
    ex_mem_reg_num = 0;
    mem_reg_num = 0;
    mem_wb_reg_num = 0;
    wb_reg_num = 0;
    misc_reg_num = 0;

    //update the simulator time, this won't change with 'b's
    mvwaddstr(sim_time_win,1,1,timebuffer[history_num]);
    wrefresh(sim_time_win);

    //tell the parent application we're ready to move on
    return(1); 
  }
  return(0);
}

//this initializes a ncurses window and sets up the arrays for exchanging reg information
extern "C" void initcurses(int if_regs, int if_id_regs, int id_regs, int id_ex_regs, int ex_regs,
                int ex_mem_regs, int mem_regs, int mem_wb_regs, int wb_regs,
                int misc_regs){
  int nbytes;
  int ready_val;

  done_state = 0;
  echo_data = 1;
  num_misc_regs = misc_regs;
  num_if_regs = if_regs;
  num_if_id_regs = if_id_regs;
  num_id_regs = id_regs;
  num_id_ex_regs = id_ex_regs;
  num_ex_regs = ex_regs;
  num_ex_mem_regs = ex_mem_regs;
  num_mem_regs = mem_regs;
  num_mem_wb_regs = mem_wb_regs;
  num_wb_regs = wb_regs;
  pid_t childpid;
  pipe(readpipe);
  pipe(writepipe);
  stdout_save = dup(1);
  childpid = fork();
  if(childpid == 0){
    close(PARENT_WRITE);
    close(PARENT_READ);
    fp = fdopen(CHILD_READ, "r");
    fp2 = fopen("program.out","w");

    //allocate room on the heap for the reg data
    inst_contents     = (char**) malloc(NUM_HISTORY*sizeof(char*));
    arf_contents      = (char**) malloc(NUM_HISTORY*sizeof(char*));
    int i=0;
    if_contents       = (char***) malloc(NUM_HISTORY*sizeof(char**));
    if_id_contents    = (char***) malloc(NUM_HISTORY*sizeof(char**));
    id_contents       = (char***) malloc(NUM_HISTORY*sizeof(char**));
    id_ex_contents    = (char***) malloc(NUM_HISTORY*sizeof(char**));
    ex_contents       = (char***) malloc(NUM_HISTORY*sizeof(char**));
    ex_mem_contents   = (char***) malloc(NUM_HISTORY*sizeof(char**));
    mem_contents      = (char***) malloc(NUM_HISTORY*sizeof(char**));
    mem_wb_contents   = (char***) malloc(NUM_HISTORY*sizeof(char**));
    wb_contents       = (char***) malloc(NUM_HISTORY*sizeof(char**));
    misc_contents     = (char***) malloc(NUM_HISTORY*sizeof(char**));
    timebuffer        = (char**) malloc(NUM_HISTORY*sizeof(char*));
    cycles            = (char**) malloc(NUM_HISTORY*sizeof(char*));
    clocks            = (char*) malloc(NUM_HISTORY*sizeof(char));
    resets            = (char*) malloc(NUM_HISTORY*sizeof(char));

    // allocate room for the register names (what is displayed)
    if_reg_names      = (char**) malloc(num_if_regs*sizeof(char*));
    if_id_reg_names   = (char**) malloc(num_if_id_regs*sizeof(char*));
    id_reg_names      = (char**) malloc(num_id_regs*sizeof(char*));
    id_ex_reg_names   = (char**) malloc(num_id_ex_regs*sizeof(char*));
    ex_reg_names      = (char**) malloc(num_ex_regs*sizeof(char*));
    ex_mem_reg_names  = (char**) malloc(num_ex_mem_regs*sizeof(char*));
    mem_reg_names     = (char**) malloc(num_mem_regs*sizeof(char*));
    mem_wb_reg_names  = (char**) malloc(num_mem_wb_regs*sizeof(char*));
    wb_reg_names      = (char**) malloc(num_wb_regs*sizeof(char*));
    misc_reg_names    = (char**) malloc(num_misc_regs*sizeof(char*));

    int j=0;
    for(;i<NUM_HISTORY;i++){
      timebuffer[i]       = (char*) malloc(8);
      cycles[i]           = (char*) malloc(7);
      inst_contents[i]    = (char*) malloc(NUM_STAGES*10);
      arf_contents[i]     = (char*) malloc(NUM_ARF*20);
      if_contents[i]      = (char**) malloc(num_if_regs*sizeof(char*));
      if_id_contents[i]   = (char**) malloc(num_if_id_regs*sizeof(char*));
      id_contents[i]      = (char**) malloc(num_id_regs*sizeof(char*));
      id_ex_contents[i]   = (char**) malloc(num_id_ex_regs*sizeof(char*));
      ex_contents[i]      = (char**) malloc(num_ex_regs*sizeof(char*));
      ex_mem_contents[i]  = (char**) malloc(num_ex_mem_regs*sizeof(char*));
      mem_contents[i]     = (char**) malloc(num_mem_regs*sizeof(char*));
      mem_wb_contents[i]  = (char**) malloc(num_mem_wb_regs*sizeof(char*));
      wb_contents[i]      = (char**) malloc(num_wb_regs*sizeof(char*));
      misc_contents[i]    = (char**) malloc(num_misc_regs*sizeof(char*));
    }
    setup_gui(fp);

    // Main loop for retrieving data and taking commands from user
    char quit_flag = 0;
    char resp=0;
    char running=0;
    int mem_addr=0;
    char goto_flag = 0;
    char cycle_flag = 0;
    char done_received = 0;
    memset(readbuffer,'\0',sizeof(readbuffer));
    while(!quit_flag){
      if (!done_received) {
        fgets(readbuffer, sizeof(readbuffer), fp);
        ready_val = processinput();
      }
      if(strcmp(readbuffer,"DONE") == 0) {
        done_received = 1;
        done_time = history_num - 1;
      }
      if(ready_val == 1 || done_received == 1){
        if(echo_data == 0 && done_received == 1) {
          running = 0;
          timeout(-1);
          echo_data = 1;
          history_num--;
          history_num%=NUM_HISTORY;
        }
        if(echo_data != 0){
          parsedata(history_num);
        }
        history_num++;
        // keep track of whether time wrapped around yet
        if (history_num == NUM_HISTORY)
          time_wrapped = 1;
        history_num%=NUM_HISTORY;

        //we're done reading the reg values for this iteration
        if (done_received != 1) {
          write(CHILD_WRITE, "n", 1);
          write(CHILD_WRITE, &mem_addr, 2);
        }
        char continue_flag = 0;
        int hist_num_temp = (history_num-1)%NUM_HISTORY;
        if (history_num==0) hist_num_temp = NUM_HISTORY-1;
        char echo_data_tmp,continue_flag_tmp;

        while(continue_flag == 0){
          resp=getch();
          if(running == 1){
            continue_flag = 1;
          }
          if(running == 0 || resp == 'p'){ 
            if(resp == 'n' && hist_num_temp == (history_num-1)%NUM_HISTORY){
              if (!done_received)
                continue_flag = 1;
            }else if(resp == 'n'){
              //forward in time, but not up to present yet
              hist_num_temp++;
              hist_num_temp%=NUM_HISTORY;
              parsedata(hist_num_temp);
            }else if(resp == 'r'){
              echo_data = 0;
              running = 1;
              timeout(0);
              continue_flag = 1;
            }else if(resp == 'p'){
              echo_data = 1;
              timeout(-1);
              running = 0;
              parsedata(hist_num_temp);
            }else if(resp == 'q'){
              //quit
              continue_flag = 1;
              quit_flag = 1;
            }else if(resp == 'b'){
              //We're goin BACK IN TIME, woohoo!
              // Make sure not to wrap around to NUM_HISTORY-1 if we don't have valid
              // data there (time_wrapped set to 1 when we wrap around to history 0)
              if (hist_num_temp > 0) {
                hist_num_temp--;
                parsedata(hist_num_temp);
              } else if (time_wrapped == 1) {
                hist_num_temp = NUM_HISTORY-1;
                parsedata(hist_num_temp);
              }
            }else if(resp == 'g' || resp == 'c'){
              // See if user wants to jump to clock cycle instead of sim time
              cycle_flag = (resp == 'c');

              // go to specified simulation time (either in history or
              // forward in simulation time).
              stop_time = get_time();
              
              // see if we already have that time in history
              int tmp_time;
              int cur_time;
              int delta;
              if (cycle_flag)
                sscanf(cycles[hist_num_temp], "%u", &cur_time);
              else
                sscanf(timebuffer[hist_num_temp], "%u", &cur_time);
              delta = (stop_time > cur_time) ? 1 : -1;
              if ((hist_num_temp+delta)%NUM_HISTORY != history_num) {
                tmp_time=hist_num_temp;
                i= (hist_num_temp+delta >= 0) ? (hist_num_temp+delta)%NUM_HISTORY : NUM_HISTORY-1;
                while (i!=history_num) {
                  if (cycle_flag)
                    sscanf(cycles[i], "%u", &cur_time);
                  else
                    sscanf(timebuffer[i], "%u", &cur_time);
                  if ((delta == 1 && cur_time >= stop_time) ||
                      (delta == -1 && cur_time <= stop_time)) {
                    hist_num_temp = i;
                    parsedata(hist_num_temp);
                    stop_time = 0;
                    break;
                  }

                  if ((i+delta) >=0)
                    i = (i+delta)%NUM_HISTORY;
                  else {
                    if (time_wrapped == 1)
                      i = NUM_HISTORY - 1;
                    else {
                      parsedata(hist_num_temp);
                      stop_time = 0;
                      break;
                    }
                  }
                }
              }

              // If we looked backwards in history and didn't find stop_time
              // then give up
              if (i==history_num && (delta == -1 || done_received == 1))
                stop_time = 0;

              // Set flags so that we run forward in the simulation until
              // it either ends, or we hit the desired time
              if (stop_time > 0) {
                // grab current values
                echo_data = 0;
                running = 1;
                timeout(0);
                continue_flag = 1;
                goto_flag = 1;
              }
            }
          }
        }
        // if we're instructed to goto specific time, see if we're there
        int cur_time=0;
        if (goto_flag==1) {
          if (cycle_flag)
            sscanf(cycles[hist_num_temp], "%u", &cur_time);
          else
            sscanf(timebuffer[hist_num_temp], "%u", &cur_time);
          if ((cur_time >= stop_time) ||
              (strcmp(readbuffer,"DONE")==0) ) {
            goto_flag = 0;
            echo_data = 1;
            running = 0;
            timeout(-1);
            continue_flag = 0;
            //parsedata(hist_num_temp);
          }
        }
      }
    }
    refresh();
    delwin(title_win);
    endwin();
    fflush(stdout);
    if(resp == 'q'){
      fclose(fp2);
      write(CHILD_WRITE, "Z", 1);
      exit(0);
    }
    readbuffer[0] = 0;
    while(strncmp(readbuffer,"DONE",4) != 0){
      if(fgets(readbuffer, sizeof(readbuffer), fp) != NULL)
        fputs(readbuffer, fp2);
    }
    fclose(fp2);
    fflush(stdout);
    write(CHILD_WRITE, "Z", 1);
    printf("Child Done Execution\n");
    exit(0);
  } else {
    close(CHILD_READ);
    close(CHILD_WRITE);
    dup2(PARENT_WRITE, 1);
    close(PARENT_WRITE);
    
  }
}


// Function to make testbench block until debugger is ready to proceed
extern "C" int waitforresponse(){
  static int mem_start = 0;
  char c=0;
  while(c!='n' && c!='Z') read(PARENT_READ,&c,1);
  if(c=='Z') exit(0);
  mem_start = read(PARENT_READ,&c,1);
  mem_start = mem_start << 8 + read(PARENT_READ,&c,1);
  return(mem_start);
}

extern "C" void flushpipe(){
  char c=0;
  read(PARENT_READ, &c, 1);
}

// Function to return string representation of opcode given inst encoding
char *get_opcode_str(int inst, int valid_inst)
{
  int opcode, check;
  char *str;
  
  if (valid_inst == ((int)'x' - (int)'0'))
    str = "-";
  else if(!valid_inst)
    str = "-";
  else if(inst==NOOP_INST)
    str = "nop";
  else {
    inst_t dummy_inst;
    dummy_inst.decode(inst);
    str = const_cast<char*>(dummy_inst.str); // due to legacy code..
  }

  return str;
}

// Function to parse register $display() from testbench and add to
// names/contents arrays
void parse_register(char *readbuf, int reg_num, char*** contents, char** reg_names) {
  char name_buf[32];
  char val_buf[32];
  int tmp_len;

  sscanf(readbuf,"%*c%s %d:%s",name_buf,&tmp_len,val_buf);
  int i=0;
  for (;i<NUM_HISTORY;i++){
    contents[i][reg_num] = (char*) malloc((tmp_len+1)*sizeof(char));
  }
  strcpy(contents[history_num][reg_num],val_buf);
  reg_names[reg_num] = (char*) malloc((strlen(name_buf)+1)*sizeof(char));
  strncpy(reg_names[reg_num], readbuf+1, strlen(name_buf));
  reg_names[reg_num][strlen(name_buf)] = '\0';
}

// Ask user for simulation time to stop at
// Since the enter key isn't detected, user must press 'g' key
//  when finished entering a number.
int get_time() {
  int col = COLS/2-6;
  wattron(title_win,A_REVERSE);
  mvwprintw(title_win,1,col,"goto time: ");
  wrefresh(title_win);
  int resp=0;
  int ptr = 0;
  char buf[32];
  int i;
  
  resp=wgetch(title_win);
  while(resp != 'g' && resp != KEY_ENTER && resp != ERR && ptr < 6) {
    if (isdigit((char)resp)) {
      waddch(title_win,(char)resp);
      wrefresh(title_win);
      buf[ptr++] = (char)resp;
    }
    resp=wgetch(title_win);
  }

  // Clean up title window
  wattroff(title_win,A_REVERSE);
  mvwprintw(title_win,1,col,"           ");
  for(i=0;i<ptr;i++)
    waddch(title_win,' ');

  wrefresh(title_win);

  buf[ptr] = '\0';
  return atoi(buf);
}
