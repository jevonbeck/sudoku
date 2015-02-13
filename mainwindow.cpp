#include "mainwindow.h"

MainWindow::MainWindow()
 {
	 QWidget *central=new QWidget(this);
	 	 
	 QGroupBox *groupBox=new QGroupBox(central);
	 groupBox->setTitle(tr("Options"));	 
	 groupBox->setFlat(true);
	 errorCheckBox= new QCheckBox(tr("Auto-Error Checking"),central);
	 hintCheckBox= new QCheckBox(tr("Enable Hints"),central);
	 submitButton=new QPushButton(tr("Submit Puzzle"),central);
	 clock=new Timer;	 
	 QVBoxLayout *vbox=new QVBoxLayout(groupBox);
	 vbox->addWidget(errorCheckBox);
	 vbox->addWidget(hintCheckBox);
	 vbox->addWidget(submitButton);
	 vbox->addWidget(clock);
	 groupBox->setLayout(vbox);

	 grid= new Grid(central);
	 grid->setDisabled(true);

	 pauseWidget= new QLabel(central);
	 pauseWidget->resize(grid->height(),grid->height());
	 pauseWidget->setAutoFillBackground(true);
	 QImage image=QImage(grid->height(),grid->height(),QImage::Format_ARGB32_Premultiplied);
	 image.fill(QColor(Qt::black).rgb());
	 QPainter painter;
	 painter.begin(&image);
	 QColor colour=QColor(Qt::red);
	 QBrush brush=QBrush(colour);
	 QFont font("Arial", 40);
	 painter.setBrush(brush);
	 painter.setPen(colour);
	 painter.setFont(font);
	 painter.drawText(0,0,grid->height(),grid->height(),Qt::AlignCenter,tr("PAUSED"));
	 painter.end();
	 pauseWidget->setPixmap(QPixmap::fromImage(image));;
	 pauseWidget->hide();

	 mainLayout=new QHBoxLayout(central);
	 mainLayout->addWidget(grid);
	 mainLayout->addWidget(groupBox);
	 central->setLayout(mainLayout);

	 editor=new QDialog(this);
	 editGrid= new Grid(editor);
	 editGrid->toEditor();
	 editMenuBar=new QMenuBar(editor);
	 editLayout=new QVBoxLayout(editor);
	 editLayout->addWidget(editMenuBar);
	 editLayout->addWidget(editGrid);
	 editLayout->setSpacing(0);
	 editor->setLayout(editLayout);
	 
         connect(submitButton,SIGNAL(pressed()),grid,SLOT(puzzleSubmitted()));
	 setCentralWidget(central);
	 setWindowTitle(tr("Sudoku"));
	 
	 createActions();
	 createMenus();	 

	 connect(grid,SIGNAL(puzzleSolved()),clock,SLOT(stopTime()));
	 connect(grid,SIGNAL(puzzleSolved()),this,SLOT(endGame()));

	 connect(errorCheckBox,SIGNAL(toggled(bool)),errorAct,SLOT(setChecked(bool)));
	 connect(errorAct,SIGNAL(toggled(bool)),errorCheckBox,SLOT(setChecked(bool)));

	 connect(this,SIGNAL(triggerGrid()),grid,SLOT(error_check()));
	 connect(this,SIGNAL(stopClock()),clock,SLOT(stopTime()));
	 
	 connect(hintCheckBox,SIGNAL(toggled(bool)),hintAct,SLOT(setChecked(bool)));
	 connect(hintAct,SIGNAL(toggled(bool)),hintCheckBox,SLOT(setChecked(bool)));

	 connect(grid,SIGNAL(resetTimer()),clock,SLOT(startTime()));
 }

 // This function creates all the actions and associated functionality to allow the game to operate as desired

 void MainWindow::createActions(){
	 newgameAct=new QAction(tr("&New Game"), this);   
   connect(newgameAct, SIGNAL(triggered()), this, SLOT(newGame()));
	 connect(newgameAct, SIGNAL(triggered()), clock, SLOT(startTime()));

	 opengameAct=new QAction(tr("&Open Game"), this);
	 connect(opengameAct, SIGNAL(triggered()), grid, SLOT(openGame()));
	 
	 pausegameAct=new QAction(tr("&Pause Game"), this);
	 pausegameAct->setDisabled(true);	 
   connect(pausegameAct, SIGNAL(triggered()), this, SLOT(pauseGame()));
	 
	 resumegameAct=new QAction(tr("&Resume Game"), this);
	 resumegameAct->setDisabled(true);
   connect(resumegameAct, SIGNAL(triggered()), clock, SLOT(startTime()));
	 connect(resumegameAct, SIGNAL(triggered()), this, SLOT(resumeGame()));

	 savegameAct=new QAction(tr("&Save Game"), this);
	 savegameAct->setDisabled(true);
	 connect(savegameAct, SIGNAL(triggered()), this, SLOT(saveGame()));
	 connect(this, SIGNAL(timeSaved(QString)), grid, SLOT(saveGame(QString)));

	 exitgameAct=new QAction(tr("&Exit Game"), this);   
   connect(exitgameAct, SIGNAL(triggered()), this, SLOT(close()));


	 easyAct=new QAction(tr("&Easy"), this);
	 easyAct->setCheckable(true);
	 easyAct->setChecked(true);
	 connect(this,SIGNAL(easy()),grid, SLOT(easy()));
	
   intermediateAct=new QAction(tr("&Intermediate"), this);
	 intermediateAct->setCheckable(true);
	 connect(this,SIGNAL(intermediate()),grid, SLOT(intermediate()));

	 hardAct=new QAction(tr("&Hard"), this);
	 hardAct->setCheckable(true);
	 connect(this,SIGNAL(hard()),grid, SLOT(hard()));
	 
	 difficultyGroup=new QActionGroup(this);
	 difficultyGroup->addAction(easyAct);
	 difficultyGroup->addAction(intermediateAct);
	 difficultyGroup->addAction(hardAct);
	 

	 puzzleEditorAct=new QAction(tr("&Puzzle Editor..."),this);
	 connect(puzzleEditorAct, SIGNAL(triggered()), this, SLOT(newPuzzle()));
	 connect(puzzleEditorAct, SIGNAL(triggered()), editGrid, SLOT(clearGrid()));
	 
	 newpuzzleAct=new QAction(tr("&New"),this);
	 connect(newpuzzleAct, SIGNAL(triggered()), editGrid, SLOT(clearGrid()));
	 
	 openpuzzleAct=new QAction(tr("&Open..."),this);
	 connect(openpuzzleAct, SIGNAL(triggered()), editGrid, SLOT(openPuzzle()));

	 loadpuzzleAct=new QAction(tr("&Load"),this);
	 connect(loadpuzzleAct, SIGNAL(triggered()), grid, SLOT(clearGrid()));
	 connect(loadpuzzleAct,SIGNAL(triggered()),this,SLOT(loadPuzzle()));
	 connect(loadpuzzleAct, SIGNAL(triggered()), clock, SLOT(startTime()));	 

   savepuzzleAct=new QAction(tr("&Save"),this);
	 connect(savepuzzleAct, SIGNAL(triggered()), editGrid, SLOT(savePuzzle()));

	 puzzleValidateAct=new QAction(tr("&Validate"),this);
	 connect(puzzleValidateAct, SIGNAL(triggered()), editGrid, SLOT(validate()));

	 validateAct=new QAction(tr("&Validate Position"),this);
	 validateAct->setDisabled(true);
	 connect(validateAct, SIGNAL(triggered()), grid, SLOT(validate()));

   solveAct=new QAction(tr("&Solve Position"),this);
	 solveAct->setDisabled(true);
	 connect(solveAct, SIGNAL(triggered()), grid, SLOT(solve()));
	 connect(solveAct, SIGNAL(triggered()), clock, SLOT(stopTime()));
	 
	 exitpuzzleAct=new QAction(tr("&Exit"), this);   
   connect(exitpuzzleAct, SIGNAL(triggered()), editor, SLOT(accept()));
	 
	 hintAct=new QAction(tr("&Hints"),this);
	 hintAct->setCheckable(true);
         hintAct->setChecked(false);
	 connect(hintAct, SIGNAL(toggled(bool)), grid, SLOT(enableHints()));

	 
	 errorAct=new QAction(tr("&Auto Error-Check"),this);
	 errorAct->setCheckable(true);
         errorAct->setChecked(false);
	 connect(errorAct, SIGNAL(toggled(bool)), grid, SLOT(enableErrors(bool)));


	 disableColourAct=new QAction(tr("&Disabled Colour"),this);
	 connect(disableColourAct, SIGNAL(triggered()), grid, SLOT(changeColours()));

	 errorColourAct=new QAction(tr("&Error Colour"),this);
	 connect(errorColourAct, SIGNAL(triggered()), grid, SLOT(changeColours()));

	 fontColourAct=new QAction(tr("&Font Colour"),this);
	 connect(fontColourAct, SIGNAL(triggered()), grid, SLOT(changeColours()));

	 normalColourAct=new QAction(tr("&Square Colour"),this);
	 connect(normalColourAct, SIGNAL(triggered()), grid, SLOT(changeColours()));

	 solveColourAct=new QAction(tr("&Solve Colour"),this);
	 connect(solveColourAct, SIGNAL(triggered()), grid, SLOT(changeColours()));

 }

 // This function creates all the menus needed to make up the game window.

 void MainWindow::createMenus(){
	 gameMenu=menuBar()->addMenu(tr("&Game"));
	 gameMenu->addAction(newgameAct);
   gameMenu->addAction(opengameAct);
	 gameMenu->addAction(pausegameAct);
	 gameMenu->addAction(resumegameAct);
   gameMenu->addAction(savegameAct);  
   gameMenu->addSeparator();
   gameMenu->addAction(exitgameAct);

	 difficultyMenu=menuBar()->addMenu(tr("&Difficulty"));
	 difficultyMenu->addAction(easyAct);
	 difficultyMenu->addAction(intermediateAct);
	 difficultyMenu->addAction(hardAct);

   puzzleMenu=menuBar()->addMenu(tr("&Puzzle"));
	 puzzleMenu->addAction(puzzleEditorAct);
	 puzzleMenu->addAction(validateAct);
	 puzzleMenu->addAction(solveAct);

	 optionsMenu=menuBar()->addMenu(tr("&Options"));
	 optionsMenu->addAction(hintAct);
	 optionsMenu->addAction(errorAct);

	 colourMenu=optionsMenu->addMenu(tr("&Colour Options"));
	 colourMenu->addAction(disableColourAct);
	 colourMenu->addAction(errorColourAct);
	 colourMenu->addAction(fontColourAct);
	 colourMenu->addAction(normalColourAct);	
	 colourMenu->addAction(solveColourAct);

	 editorPuzzleMenu=editMenuBar->addMenu(tr("&Puzzle"));
	 editorPuzzleMenu->addAction(newpuzzleAct);
	 editorPuzzleMenu->addAction(openpuzzleAct);
	 editorPuzzleMenu->addAction(loadpuzzleAct);
	 editorPuzzleMenu->addAction(savepuzzleAct);
	 editorPuzzleMenu->addAction(puzzleValidateAct);
	 editorPuzzleMenu->addAction(exitpuzzleAct);	
 }

 // This function triggers the appropriate Difficulty Level signal in order
 // to generate the appropriate level puzzle.

 void MainWindow::newGame(){	 
	 openGame();
	 resumegameAct->setEnabled(false);
	 pausegameAct->setEnabled(true);

	 hintAct->setEnabled(true);
	 errorAct->setEnabled(true);

	 QAction *act=difficultyGroup->checkedAction();
	 if(act==easyAct){
		 emit easy();		 
	 }
	 else{
		 if(act==intermediateAct){
			 emit intermediate();
		 }
		 else{
			 emit hard();
		 }
	 }	 	 	 
 }

 void MainWindow::saveGame(){
	 emit timeSaved(clock->TimeString());
 }

 // This function opens a file and loads the grid based on the contents of the file.

 void MainWindow::openGame(){
	 grid->setEnabled(true);
	 savegameAct->setEnabled(true);
	 solveAct->setEnabled(true);
	 validateAct->setEnabled(true);
 }

 // This function opens the Puzzle Editor and 
 // if a game was being played while calling the Puzzle Editor, the game is paused.

 void MainWindow::newPuzzle(){
	 if(clock->isTiming()){
		 pauseGame();		 
	 }
	 editor->exec();
 }

 // This function loads the puzzle from the Puzzle Editor to the Game grid

 void MainWindow::loadPuzzle(){
	 grid->setEnabled(true);
	 validateAct->setEnabled(true);
	 solveAct->setEnabled(true);
	 resumeGame();
	 QList<square_info> puzzle=editGrid->currentGrid();
	 for (QList<square_info>::iterator x=puzzle.begin(); x!=puzzle.end(); ++x){
		 if((*x).value!=0){
		   (*x).enabled=0;
		 }
	 }
	 grid->populauteGrid(puzzle);
	 editor->accept();
 }

 // This function removes the pause widget and shows the grid, 
 // while appropriately enabling/disabling the other relevant actions

 void MainWindow::resumeGame(){
	 resumegameAct->setEnabled(false);
	 pausegameAct->setEnabled(true);
	 savegameAct->setEnabled(true);
	 validateAct->setEnabled(true);
	 solveAct->setEnabled(true);

	 if(mainLayout->itemAt(0)->widget()==pauseWidget){
	   pauseWidget->hide();
	   mainLayout->removeWidget(pauseWidget);
	   mainLayout->insertWidget(0,grid);
	 
	   grid->show();
	   grid->setFocus();
	 }
 }

 // This function removes the grid and shows the pause widget,
 // while appropriately enabling/disabling the other relevant actions

 void MainWindow::pauseGame(){
	 emit stopClock();
	 pausegameAct->setEnabled(false);
	 resumegameAct->setEnabled(true);
	 savegameAct->setEnabled(false);
	 validateAct->setEnabled(false);
	 solveAct->setEnabled(false);

	 grid->hide();
	 mainLayout->removeWidget(grid);
	 mainLayout->insertWidget(0,pauseWidget);
	 
	 pauseWidget->show();
	 pauseWidget->setFocus();
 }

 // This function continuously triggers the grid so that if automatic error-checking is enabled
 // the squares will be updated automatically.

 bool MainWindow::event(QEvent *event){
	 emit triggerGrid();
	 return QWidget::event(event);
 }


 void MainWindow::endGame(){
	 QMessageBox::information(this, tr("Sudoku"), (tr("Congratulations!!! You solved the puzzle. \n Your time is: ")+(clock->Time()).toString("hh:mm:ss")));
	 grid->setDisabled(true);
 }
