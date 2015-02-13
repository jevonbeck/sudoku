#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QMenuBar>
#include <QActionGroup>
#include <QLayoutItem>
#include "grid.h"
#include "timer.h"


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

protected:
	bool event(QEvent *event);

signals:
	void easy(void);
	void intermediate(void);
	void hard(void);
	void triggerGrid(void);
	void stopClock(void);
	void timeSaved(QString time);

private slots:
	void newGame();
	void openGame();
	void saveGame();
	void resumeGame();
	void pauseGame();
	void newPuzzle();
	void loadPuzzle();
	void endGame();

private:
  void createActions();
  void createMenus();
  
	QMenu *gameMenu;
  QMenu *difficultyMenu;
  QMenu *puzzleMenu;
	QMenu *optionsMenu;
	QMenu *colourMenu;
	QMenu *editorPuzzleMenu;

  QToolBar *gameToolBar;
  QToolBar *difficultyToolBar;
	QToolBar *puzzleToolBar;
  QToolBar *optionsToolBar;

  QAction *newgameAct;
  QAction *opengameAct;
	QAction *pausegameAct;
	QAction *resumegameAct;
  QAction *savegameAct;  
  QAction *exitgameAct;

	QActionGroup *difficultyGroup;
	QAction *easyAct;
  QAction *intermediateAct;
	QAction *hardAct;

	QAction *newpuzzleAct;
  QAction *openpuzzleAct;
	QAction *loadpuzzleAct;
  QAction *savepuzzleAct;
	QAction *puzzleEditorAct;
	QAction *validateAct;
  QAction *solveAct;
	QAction *exitpuzzleAct;

	QAction *hintAct;
	QAction *errorAct;

	QAction *disableColourAct;
	QAction *errorColourAct;
	QAction *fontColourAct;
	QAction *normalColourAct;	
	QAction *solveColourAct;		

	QCheckBox *hintCheckBox;
	QCheckBox *errorCheckBox;
	QPushButton *submitButton;
	QHBoxLayout *mainLayout;
	Grid *grid;
	Timer *clock;
	QLabel *pauseWidget;

	QDialog *editor;
	Grid *editGrid;
	QMenuBar *editMenuBar;
	QVBoxLayout *editLayout;
	QAction *puzzleValidateAct;
};

#endif // MAINWINDOW_H