#include "grid.h"

Grid::Grid(QWidget *parent)
     : QWidget(parent)
 {
	 QGridLayout *gridLayout = new QGridLayout;
	 for(int p=0; p!=3; ++p){
		 for(int q=0; q!=3; ++q){
       QGridLayout *gridLayoutTemp = new QGridLayout;		
			 for(int row=0; row!=3; ++row){
	       for(int col=0; col!=3; ++col){
		       reference[3*p+row][3*q+col]=new square;
					 connect(this,SIGNAL(resetSquare()),reference[3*p+row][3*q+col],SLOT(clearSquare()));
					 connect(this,SIGNAL(makeEditor()),reference[3*p+row][3*q+col],SLOT(enableEditor()));
		       gridLayoutTemp->addWidget(reference[3*p+row][3*q+col],row,col);
	       }
	     }
	     gridLayoutTemp->setSpacing(0);
	     gridLayout->addLayout(gridLayoutTemp,p,q);
		 }
	 }
	 int space_size=3;
	 gridLayout->setSpacing(space_size);	 

	 int height1=9*reference[0][0]->height()+8*space_size+2*space_size;
	 int width1=height1;
	 setLayout(gridLayout);
	 QSize grid_size=QSize(width1,height1);
	 resize(grid_size);
	 setMinimumSize(grid_size);
	 setMaximumSize(grid_size);
         errorEnable=false;
         hintEnable=false;
}

// This function determines if a number exists in a particular row

bool Grid::row_has_num(int row_num, int number){
	for (int x=0; x!=9; ++x){
		if (reference[row_num][x]->Value()==number){
      return true;
		}
	}
  return false;
}


// This function determines if a number exists in a particular column

bool Grid::col_has_num(int col_num, int number){
	for (int x=0; x!=9; ++x){
		if (reference[x][col_num]->Value()==number){
      return true;
		}
	}
  return false;
}


// This function determines if a number exists in a particaular 3x3 block

bool Grid::blk_has_num(int blk_row,int blk_col,int number){
	for (int row=0; row!=3; ++row){
		for (int col=0; col!=3; ++col){
			if (reference[3*blk_row+row][3*blk_col+col]->Value()==number){
        return true;
			}
		}
	}
  return false;
}

// This function changes the colour of a square 
// depending on whether or not the square value makes a valid puzzle.

void Grid::highlight_errors(){
	for (int row=0; row!=9; ++row){
    for (int col=0; col!=9; ++col){
			if(reference[row][col]->isEnabled()){
			  if (reference[row][col]->Value()!=0){ // if an editable square has a value
			    int temp_val=reference[row][col]->Value();
          reference[row][col]->setValue(0);
			    QColor colour=reference[row][col]->SquareColour();

				  // detemine if the value exists elsewhere in the row, column or block associated with the current square
			    if (row_has_num(row,temp_val) || col_has_num(col,temp_val) || blk_has_num(row/3,col/3,temp_val)){					  
			      colour=reference[row][col]->ErrorColour(); // if the value exists elsewhere change colour of square
			    }
			    else{
			      colour=reference[row][col]->NormalColour(); // otherwise the square colour should be normal colour
			    }
			    reference[row][col]->setValue(temp_val);

			    if(colour!=reference[row][col]->SquareColour()){ // if the square colour changed
				    reference[row][col]->setSquareColour(colour);
			      reference[row][col]->DisplayValue(); // display square with new colour
			    }			
		    }
			  else{// if the square is blank and previously had a conflicting value
			    if(reference[row][col]->SquareColour()!=reference[row][col]->NormalColour()){
				    reference[row][col]->setSquareColour(reference[row][col]->NormalColour());
			      reference[row][col]->DisplayValue(); // return the colour of the square to normal
			    }			
		    }
			}
	  } 
	}
}

// This function determines whether or not the puzzle is correctly solved.

bool Grid::isSolved(){
	for (int row=0; row!=9; ++row){
    for (int col=0; col!=9; ++col){
			int temp_val=reference[row][col]->Value();
      reference[row][col]->setValue(0);
			if (row_has_num(row,temp_val) || col_has_num(col,temp_val) || blk_has_num(row/3,col/3,temp_val)){
				reference[row][col]->setValue(temp_val);
				return false; 
			}
			reference[row][col]->setValue(temp_val);			
		}
	}
	return true;
}

// This function determines whether or not a rule was broken. 

void Grid::error_check(){
	if(errorEnable){
		highlight_errors();
	}
	else{
		for (int row=0; row!=9; ++row){
		  for (int col=0; col!=9; ++col){
				if(reference[row][col]->SquareColour()==reference[row][col]->ErrorColour() 
					 && reference[row][col]->isEnabled()){
					reference[row][col]->setSquareColour(reference[row][col]->NormalColour());
					reference[row][col]->DisplayValue();
				}
			}
		}
	}	
}

void Grid::puzzleSubmitted(){
	if(isSolved()){
		emit puzzleSolved();		
	}
	else{
		if(full_grid()){
			QMessageBox::information(this, tr("Sudoku"), tr("The solution is incorrect!"));			
		}
		else{
			QMessageBox::information(this, tr("Sudoku"), tr("The puzzle is incomplete!"));
		}
	}
}
// This function determines all the values that are not in a particular row

QList<int> Grid::not_in_row(int row_num){
	QList<int> all_num,val_in_row;
	for (int x=1; x!=10; ++x){    
		all_num.push_back(x); //populate vector of 1 to 9
    
		// populate all non-zero values in the row
		if (reference[row_num][x-1]->Value()!=0){
			val_in_row.push_back(reference[row_num][x-1]->Value());
		}
	}

  // remove numbers found in row from vector with 1 to 9
	for (QList<int>::iterator x=all_num.begin(); x!=all_num.end(); ++x){
	  for (QList<int>::iterator y=val_in_row.begin(); y!=val_in_row.end(); ++y){		
			if (*x==*y){
				x=all_num.erase(x);
				x=--x;
				y=val_in_row.erase(y);
				y=--val_in_row.end(); // break loop				
			}
		}
	}  
  return all_num;
}


// This function determines all the values that are not in a particular column

QList<int> Grid::not_in_col(int col_num){
	QList<int> all_num,val_in_col;
	for (int x=1; x!=10; ++x){    
		all_num.push_back(x); //populate vector of 1 to 9
    
		// populate all non-zero values in the column
		if (reference[x-1][col_num]->Value()!=0){
			val_in_col.push_back(reference[x-1][col_num]->Value());
		}
	}

  // remove numbers found in column from vector with 1 to 9
	for (QList<int>::iterator x=all_num.begin(); x!=all_num.end(); ++x){
	  for (QList<int>::iterator y=val_in_col.begin(); y!=val_in_col.end(); ++y){		
			if (*x==*y){
				x=all_num.erase(x);
				x=--x;
				y=val_in_col.erase(y);
				y=--val_in_col.end(); // break loop				
			}
		}
	}  
  return all_num;
}

// This function determines all the values that are not in a particular block

QList<int> Grid::not_in_blk(int blk_row, int blk_col){
	QList<int> all_num,val_in_blk;
	for (int x=0; x!=3; ++x){
		for (int y=0; y!=3; ++y){
      all_num.push_back(3*x+y+1); //populate vector of 1 to 9

			// populate all non-zero values in the block
			if (reference[3*blk_row+x][3*blk_col+y]->Value()!=0){
				val_in_blk.push_back(reference[3*blk_row+x][3*blk_col+y]->Value());
			}
		}
	}

	// remove numbers found in block from vector with 1 to 9
	for (QList<int>::iterator x=all_num.begin(); x!=all_num.end(); ++x){
	  for (QList<int>::iterator y=val_in_blk.begin(); y!=val_in_blk.end(); ++y){		
			if (*x==*y){
				x=all_num.erase(x);
				x=--x;
				y=val_in_blk.erase(y);
				y=--val_in_blk.end(); // break loop				
			}
		}
	}

  return all_num;	
}

// This function makes a list of hints. 
// Hints are determined by first finding what numbers are not present
// in the row, column and block that a particular square exists in.
// Any number that is common to all three lists is therefore a hint

QList<int> Grid::hintListAt(int row, int col){
	QList<int> rowList=not_in_row(row);
	QList<int> colList=not_in_col(col);
	QList<int> blkList=not_in_blk(row/3,col/3);

	QList<int> hints;
	for (int x=0; x!=rowList.size(); ++x){
		for (int y=0; y!=colList.size(); ++y){
			if (rowList.at(x)==colList.at(y)){ // if an element is common to the row and column lists
				for (int z=0; z!=blkList.size(); ++z){
					if (blkList.at(z)==colList.at(y)){ // and if it is common to the block list as well
						hints.push_back(blkList.at(z)); // store element as a hint
					}
				}
			}
		}
	}
  return hints;
}

// This function turns the hint list into a string.

QString Grid::makeHint(const QList<int> &hints){
	QString hint;
	for (QList<int>::const_iterator x=hints.begin(); x!=hints.end(); ++x){
		hint+=QVariant(*x).toString()+",";
	}
	if (hints.size()>0){
		hint.remove(hint.lastIndexOf(','),1);
	}
	return hint;
}

// This function makes a hint for a particular square

void Grid::make_hintAt(int row, int col){
	if(reference[row][col]->Value()==0){
		reference[row][col]->setHint(makeHint(hintListAt(row,col)));
	}
}

// This function fills squares with values to make a valid Sudoku solution

void Grid::puzzle_gen(){
	clearGrid();
	QColor temp_colour=reference[0][0]->SolveColour();
	for (int row=0; row!=9; ++row){
	  for (int col=0; col!=9; ++col){
			reference[row][col]->setSolveColour(reference[row][col]->NormalColour());
		}
	}
	solve();
	for (int row=0; row!=9; ++row){
	  for (int col=0; col!=9; ++col){
			reference[row][col]->setSolveColour(temp_colour);
		}
	}
}


// This function generates a puzzle based on the Easy difficulty algorithm.

void Grid::easy(){
	puzzle_gen();
	
	int row_count=0;	
	

	/*for(int number=1; number!=10; ++number){		
		QList<int> all_num;
		for(int x=0;x!=9;++x){
			all_num.push_back(x);
		}

		while(all_num.size()>3){
		  QTime time=QTime::currentTime();
		  int pos=(rand()+3+time.msec()*(time.second()+rand()))%all_num.size();
			int row=all_num.at(pos);

		  for(int col=0; col!=9; ++col){
			  if(reference[row][col]->Value()==number){
				  reference[row][col]->setValue(0);
			    reference[row][col]->DisplayValue();
					all_num.removeAt(pos);
				  col=8;
			  }
		  }			
		}
	}*/
	
	/*QVector<int> number_count(9),end_result(9,6);
	int row,col;
	while(number_count!=end_result){
		// randomly choose a square in the grid
	  do {
		  QTime time=QTime::currentTime();
		  row=(rand()+3+time.msec()*(time.second()+rand()))%9;
		  time=QTime::currentTime();
		  col=(rand()+4+time.msec()*(time.second()+rand()))%9;
	  }while(reference[row][col]->Value()==0);

		// remove 6 instances of each number leaving at least 3 numbers in each block
		if(number_count[reference[row][col]->Value()-1]<6 && (not_in_blk(row/3,col/3)).size()<6){
			++number_count[reference[row][col]->Value()-1];
			reference[row][col]->setValue(0);
			reference[row][col]->DisplayValue();
		}	  
	}*/
	
	// eliminate values in each block until only 3 values remain in each block
	/*for(int row=0; row!=3; ++row){
	  for(int col=0; col!=3; ++col){
			for(int number_count=0; number_count!=7; ++number_count){
		    int guess_row, guess_col;

		    do {
			    QTime time=QTime::currentTime();
		      guess_row=(rand()+3+time.msec()*(time.second()+rand()))%3;
					time=QTime::currentTime();
				  guess_col=(rand()+4+time.msec()*(time.second()+rand()))%3;
		    }while(reference[3*row+guess_row][3*col+guess_col]->Value()==0);
		  
		    reference[3*row+guess_row][3*col+guess_col]->setValue(0);
		    //reference[3*row+guess_row][3*col+guess_col]->DisplayValue();
			}
		}
	}*/


	// eliminate 1 value in each row
	for(int row=0; row!=9; ++row){
		QTime time=QTime::currentTime();
		int pos=(rand()+3+time.msec()*(time.second()+rand()))%9; // guess a valid value
		reference[row][pos]->setValue(0);
		reference[row][pos]->DisplayValue();
	}

	// eliminate 1 value in each column
	for(int col=0; col!=9; ++col){		
		int pos;

		do {
			QTime time=QTime::currentTime();
		  pos=(rand()+3+time.msec()*(time.second()+rand()))%9;
		}while(reference[pos][col]->Value()==0);
		  
		reference[pos][col]->setValue(0);
		reference[pos][col]->DisplayValue();
	}

	// eliminate 1 value in each block
	for(int row=0; row!=3; ++row){
	  for(int col=0; col!=3; ++col){		
		  int guess_row, guess_col;

		  do {
			  QTime time=QTime::currentTime();
		    guess_row=(rand()+3+time.msec()*(time.second()+rand()))%3;
				guess_col=(rand()+3+time.msec()*(time.second()+rand()))%3;
		  }while(reference[3*row+guess_row][3*col+guess_col]->Value()==0);
		  
		  reference[3*row+guess_row][3*col+guess_col]->setValue(0);
		  reference[3*row+guess_row][3*col+guess_col]->DisplayValue();
		}
	}
    
	int Count = 3;
	while (Count != 0){
	  while (row_count < 9){
        for (int column_count = 0; column_count < 9; column_count++) {
          //while (reference[row_count][column_count]->Value() != 0){
          //looking for all positions that have just been filled
          
			QTime time=QTime::currentTime();
            int int_rand = ((rand()+4+time.msec()*(time.second()+rand()))%9)+1;
			if (reference[row_count][column_count]->Value() == int_rand){
              reference[row_count][column_count]->setValue(0);
			  reference[row_count][column_count]->DisplayValue();
			}
     
          //}

        }
        row_count = row_count + 1;
      }
	  Count = Count - 1;
	}

	// fix given square values
	for(int row=0; row!=9; ++row){
	  for(int col=0; col!=9; ++col){
			if(reference[row][col]->Value()!=0){
				reference[row][col]->disableSquare(true);
			}
		}
	}
}

// This function generates a puzzle based on the Intermediate difficulty algorithm.

void Grid::intermediate(){
	puzzle_gen();
   	
	int row_count=0;
	
	for (int count = 0; count < 2; count++){
	// eliminate 1 value in each row
	  for(int row=0; row!=9; ++row){
		QTime time=QTime::currentTime();
		int pos=(rand()+3+time.msec()*(time.second()+rand()))%9; // guess a valid value
		reference[row][pos]->setValue(0);
		reference[row][pos]->DisplayValue();
	  }

	// eliminate 1 value in each column
	  for(int col=0; col!=9; ++col){		
		int pos;

		do {
			QTime time=QTime::currentTime();
		  pos=(rand()+3+time.msec()*(time.second()+rand()))%9;
		}while(reference[pos][col]->Value()==0);
		  
		reference[pos][col]->setValue(0);
		reference[pos][col]->DisplayValue();
	  }

	// eliminate 1 value in each block
	  for(int row=0; row!=3; ++row){
	    for(int col=0; col!=3; ++col){		
		  int guess_row, guess_col;

		  do {
			  QTime time=QTime::currentTime();
		    guess_row=(rand()+3+time.msec()*(time.second()+rand()))%3;
				guess_col=(rand()+3+time.msec()*(time.second()+rand()))%3;
		  }while(reference[3*row+guess_row][3*col+guess_col]->Value()==0);
		  
		  reference[3*row+guess_row][3*col+guess_col]->setValue(0);
		  reference[3*row+guess_row][3*col+guess_col]->DisplayValue();
		}
	  }
	
	   
	  int Count = 3;
	  while (Count != 0){
	    while (row_count < 9){
          for (int column_count = 0; column_count < 9; column_count++) {
          //while (reference[row_count][column_count]->Value() != 0){
          //looking for all positions that have just been filled
          
			QTime time=QTime::currentTime();
            int int_rand = ((rand()+4+time.msec()*(time.second()+rand()))%9)+1;
			if (reference[row_count][column_count]->Value() == int_rand){
              reference[row_count][column_count]->setValue(0);
			  reference[row_count][column_count]->DisplayValue();
			}
     
          //}

          }
          row_count = row_count + 1;
        }
	    Count = Count - 1;
	  }

	}

	
	// fix given square values
	for(int row=0; row!=9; ++row){
	  for(int col=0; col!=9; ++col){
			if(reference[row][col]->Value()!=0){
				reference[row][col]->disableSquare(true);
			}
		}
	}
}

// This function generates a puzzle based on the Hard difficulty algorithm.

void Grid::hard(){
	puzzle_gen();
    
	
	for (int row=0;row<9;row+=2){
		for(int col=0;col<9;col+=2){
			if(col+1<9){
			  reference[row][col+1]->setValue(0);
		 	  reference[row][col+1]->DisplayValue();
			}

			if(col+3<9){
			  reference[row][col+3]->setValue(0);
			  reference[row][col+3]->DisplayValue();
			}

			if(col+5<9){
			  reference[row][col+5]->setValue(0);
			  reference[row][col+5]->DisplayValue();
			}

			if(col+7<9){
			  reference[row][col+7]->setValue(0);
			  reference[row][col+7]->DisplayValue();
			}

			if(row+1<9){
			  reference[row+1][col]->setValue(0);
			  reference[row+1][col]->DisplayValue();
			}

			if(row+3<9){
			  reference[row+3][col]->setValue(0);
			  reference[row+3][col]->DisplayValue();
			}

			if(row+5<9){
			  reference[row+5][col]->setValue(0);
			  reference[row+5][col]->DisplayValue();
			}

			if(row+7<9){
			  reference[row+7][col]->setValue(0);
			  reference[row+7][col]->DisplayValue();
			}
		}
	}

    // eliminate 1 value in each column
	for(int col=0; col!=9; ++col){		
		int pos;

		do {
			QTime time=QTime::currentTime();
		  pos=(rand()+3+time.msec()*(time.second()+rand()))%9;
		}while(reference[pos][col]->Value()==0);
		  
		reference[pos][col]->setValue(0);
		reference[pos][col]->DisplayValue();
	}

	// eliminate 1 value in each row
	for(int row=0; row!=9; ++row){
		QTime time=QTime::currentTime();
		int pos=(rand()+3+time.msec()*(time.second()+rand()))%9; // guess a valid value
		reference[row][pos]->setValue(0);
		reference[row][pos]->DisplayValue();
	}

	for(int row=0; row!=3; ++row){
	  for(int col=0; col!=3; ++col){		
		  int guess_row, guess_col;

		  do {
			  QTime time=QTime::currentTime();
		    guess_row=(rand()+3+time.msec()*(time.second()+rand()))%3;
				guess_col=(rand()+3+time.msec()*(time.second()+rand()))%3;
		  }while(reference[3*row+guess_row][3*col+guess_col]->Value()==0);
		  
		  reference[3*row+guess_row][3*col+guess_col]->setValue(0);
		  reference[3*row+guess_row][3*col+guess_col]->DisplayValue();
		}
	}
	

	// fix given square values
	for(int row=0; row!=9; ++row){
	  for(int col=0; col!=9; ++col){
			if(reference[row][col]->Value()!=0){
				reference[row][col]->disableSquare(true);
			}
		}
	}

}

// This function creates a list of square_info data structures from a file.
// Each data structure contains information about each square and how it is to be displayed.

QList<square_info> Grid::makeFileList(QString directory,int fieldSize){
	QList<square_info> squares;
	QString fileName;
	if(fieldSize==3){
	  fileName=QFileDialog::getOpenFileName(this,tr("Open"),directory,
		                                      tr("Sudoku Puzzle (*.puz);;Text Files (*.txt);;All Files (*)"));
	}
	else{
		if(fieldSize==4){
		  fileName=QFileDialog::getOpenFileName(this,tr("Open"),directory,
		                                        tr("Sudoku Game (*.sdk);;Text Files (*.txt);;All Files (*)"));
		}
	}

	if (!fileName.isEmpty()){
		QFile file(fileName);
		if (file.open(QFile::ReadOnly | QFile::Text)) {
			QTextStream in(&file);			
			while(!in.atEnd()){					
			  QString line=in.readLine();
				QStringList fields=line.split(',',QString::SkipEmptyParts);
				square_info square_data;
				enum {VALUE,ROW,COLUMN,ENABLE};
				if (fields.size()==fieldSize){
					square_data.value=QVariant(fields.at(VALUE)).toInt();
					square_data.row=QVariant(fields.at(ROW)).toInt();
					square_data.col=QVariant(fields.at(COLUMN)).toInt();
					if (fieldSize==3){
						square_data.enabled=0;
					}
					else{
						square_data.enabled=QVariant(fields.at(ENABLE)).toInt();
					}
					squares.push_back(square_data);
				}
			}
		}
	}
	return squares;
}

// This function uses the elements in the input list to populate the grid.

void Grid::populauteGrid(const QList<square_info>& square_values){
	clearGrid();
	for (QList<square_info>::const_iterator x=square_values.begin(); x!=square_values.end(); ++x){
    reference[(*x).row][(*x).col]->setValue((*x).value);
		if((*x).enabled==0){
      reference[(*x).row][(*x).col]->disableSquare(true);
		}
		reference[(*x).row][(*x).col]->DisplayValue();
	}	
}

// This function opens files of the .sdk file extension type and
// populates the grid with numbers according to the data in the file.

void Grid::openGame(){
	QList<square_info> square_values=makeFileList(tr("C:"),4);
	if(square_values.size()==0){
    return;
	}
	populauteGrid(square_values);
	emit resetTimer();
}

// This function opens files of the .puz file extension type and
// populates the grid with numbers according to the data in the file.

// Since the .puz file extension is specifically for puzzles, 
// all numbers entered into the grid are disabled.

void Grid::openPuzzle(){
	QList<square_info> square_values=makeFileList(tr("C:"),3);
	if(square_values.size()==0){
    return;
	}
	clearGrid();
	for (QList<square_info>::iterator x=square_values.begin(); x!=square_values.end(); ++x){
    reference[(*x).row][(*x).col]->setValue((*x).value);
		reference[(*x).row][(*x).col]->DisplayValue();
	}
}


// This function saves a file in a specified location according to the specified file type.

void Grid::saveFile(QString directory, QString file_ext){
	QString fileName;
	if (file_ext==".puz"){
	  fileName=QFileDialog::getSaveFileName(this,tr("Save"),directory,tr("Sudoku Puzzle (*.puz);;Text Files (*.txt)"));
	}
	else{
		if (file_ext==".sdk"){
		  fileName=QFileDialog::getSaveFileName(this,tr("Save"),directory,tr("Sudoku Game (*.sdk);;Text Files (*.txt)"));
		}
	}

	fileName+=file_ext;
	QFile file(fileName);
	QTextStream out(&file);
	if (!file.open(QFile::WriteOnly | QFile::Text)){
		return;
	}
	
	if (file_ext==".sdk"){
		out<<time_saved<<"\n";
	}
	
	for (int row=0; row!=9; ++row){
		for (int col=0; col!=9; ++col){
			if (reference[row][col]->Value()!=0){
				QString line=QVariant(reference[row][col]->Value()).toString()+tr(",")+
					                    QVariant(row).toString()+tr(",")+QVariant(col).toString();//;
				if(file_ext==".sdk"){
					int enable;
					if(reference[row][col]->isEnabled()){
						enable=1;
					}
					else{
						enable=0;
					}
					line+=tr(",")+QVariant(enable).toString();
				}

        out<<line<<"\n";
			}
		}
	}	
}

// This function specifically stores files in the .sdk file format.

void Grid::saveGame(QString time){
	saveFile(tr("C:"), tr(".sdk"));
	time_saved=time;
}

// This function specifically stores files in the .puz file format.

void Grid::savePuzzle(){
	saveFile(tr("C:"), tr(".puz"));	
}

// This function validates that a puzzle can be solved

void Grid::validate(){
	QList<square_info> grid=currentGrid();
	int attempt_count=0;
	int stop_number=50;

	while(!isSolved() && attempt_count!=stop_number){
		clearGrid();
		for (QList<square_info>::iterator x=grid.begin(); x!=grid.end(); ++x){
      reference[(*x).row][(*x).col]->setValue((*x).value);			  
	  }	
	  solve_attempt();
		++attempt_count;
	}

	clearGrid();
	if(attempt_count<stop_number){		
		QMessageBox::information(this, tr("Sudoku"), tr("The puzzle is solvable."));				
	}
	else{
		QMessageBox::information(this, tr("Sudoku"), tr("The puzzle is unsolvable!!!"));
	}
	for (QList<square_info>::iterator x=grid.begin(); x!=grid.end(); ++x){
    reference[(*x).row][(*x).col]->setValue((*x).value);		
		reference[(*x).row][(*x).col]->DisplayValue();
		if((*x).enabled==0){
      reference[(*x).row][(*x).col]->disableSquare(true);
		}      
  }	
}

// This function attempts to solve the puzzle by analysing the number of hints
// and making educated guesses.

void Grid::solve_attempt(){
	int hint_size=1;
	bool reduce_hint_size=false;
	while(hint_size!=10){
	  bool final_check=true;
	  while (final_check){ // while there are squares with a certain number of possible valid values to fill out
		  final_check=false;
		  for(int row=0; row!=9; ++row){ // look through the entire grid
			  for(int col=0; col!=9; ++col){
				  if(reference[row][col]->Value()==0){ // if a square is empty
						QList<int> possible_solns=hintListAt(row,col);
					  if(possible_solns.size()==hint_size){
						  if(hint_size==1){ // and there is one hint valid value for the square
						    reference[row][col]->setValue(possible_solns.at(0));
								reference[row][col]->setSquareColour(reference[row][col]->SolveColour());
						    reference[row][col]->DisplayValue();	// put the valid value in the square
						    final_check=true;
						  }
						  else{ // otherwise, if there is more than one valid value
							  if(hint_size>1){
									QTime time=QTime::currentTime();
									int pos=(rand()+3+time.msec()*(time.second()+rand()))%possible_solns.size(); // guess a valid value
							    reference[row][col]->setValue(possible_solns.at(pos));
									reference[row][col]->setSquareColour(reference[row][col]->SolveColour());
						      reference[row][col]->DisplayValue(); // and put this guessed value in the square
							    reduce_hint_size=true;
									row=8; // break loop
									col=8;
							  }							 
						  }
					  }					
				  }
			  }
		  }
	  }
		if (reduce_hint_size){ // if a guess was made, 
			--hint_size; // look for squares with one less than the previous no. of possible valid numbers
			reduce_hint_size=false;
		}
		else{ // otherwise
		  ++hint_size; // look for squares with one more than the previous number of possible values searched for
		}
	}	
}


// This function verifies that all squares in the grid are filled
// with non-zero numbers.

bool Grid::full_grid(){	
	for(int row=0; row!=3; ++row){
		for(int col=0; col!=3; ++col){
			if(blk_has_num(row,col,0)){
				return false;								
			}
		}
	}
	return true;
}


// This function stores the current state of the grid

QList<square_info> Grid::currentGrid(){
	QList<square_info> grid;
	square_info store;
	for(int row=0; row!=9; ++row){
		for(int col=0; col!=9; ++col){
			store.value=reference[row][col]->Value();
			store.row=row;
			store.col=col;
			store.enabled=1;
			if(!reference[row][col]->isEnabled()){
				store.enabled=0;
			}
			grid.push_back(store);
		}
	}
	return grid;
}

// This function solves the puzzle on the grid.
// If after 20 solve attempts and the puzzle is unsolved, 
// it reports that the puzzle could not be solved

void Grid::solve(){
	QList<square_info> grid=currentGrid();
	int attempt_count=0;
	int stop_number=50;

	while(!full_grid() && attempt_count!=stop_number){
		clearGrid();
		for (QList<square_info>::iterator x=grid.begin(); x!=grid.end(); ++x){
      reference[(*x).row][(*x).col]->setValue((*x).value);
			reference[(*x).row][(*x).col]->DisplayValue();
			if((*x).enabled==0){
        reference[(*x).row][(*x).col]->disableSquare(true);
			}      
	  }	
	  solve_attempt();
		++attempt_count;
	}

	if(attempt_count==stop_number){
		QMessageBox::information(this, tr("Sudoku"), tr("The puzzle is unsolvable!!!"));
		clearGrid();
		for (QList<square_info>::iterator x=grid.begin(); x!=grid.end(); ++x){
      reference[(*x).row][(*x).col]->setValue((*x).value);
			reference[(*x).row][(*x).col]->DisplayValue();
			if((*x).enabled==0){
        reference[(*x).row][(*x).col]->disableSquare(true);
			}      
	  }	
	}
}

// This function enables the hints to be shown in the unfilled squares.

void Grid::enableHints(){
	QObject *obj=sender();
	QAction *act=qobject_cast<QAction *>(obj);
	hintEnable=act->isChecked();	
}

// This function enables automatic error-checking

void Grid::enableErrors(bool enable){
	errorEnable=enable;
}

// This function returns a pointer to the square at the position clicked.

square *Grid::itemAt(const QPoint &pos){
	QWidget *child=childAt(pos);
	square *block=qobject_cast<square *>(child);
	return block;
}

// This function sets the content of the tool-tip if hints are enabled

bool Grid::event(QEvent *event){
	if (event->type() == QEvent::ToolTip && hintEnable==true) {
    QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		square *block= itemAt(helpEvent->pos());

		if (block!=0 && block->Value()==0){
      for(int row=0; row!=9; ++row){
				for(int col=0; col!=9; ++col){
					make_hintAt(row,col);
				}
			}
			QToolTip::showText(helpEvent->globalPos(), block->Hint());
		}
		else {
      QToolTip::showText(helpEvent->globalPos(), tr(""));
		}
  }
	return QWidget::event(event);	
}

// This function changes the individual colours that the block can assume under different conditions.

void Grid::changeColours(){
	QColor colour = QColorDialog::getColor(Qt::red, this);
	QObject *obj=sender();
	QAction *act=qobject_cast<QAction *>(obj);
	for(int row=0; row!=9; ++row){
		for(int col=0; col!=9; ++col){
      if(act->text()=="&Disabled Colour"){
				reference[row][col]->setDisableColour(colour);
			}
			else{
				if(act->text()=="&Error Colour"){
					reference[row][col]->setErrorColour(colour);
				}
				else{
					if(act->text()=="&Font Colour"){
						reference[row][col]->setFontColour(colour);
					}
					else{
						if(act->text()=="&Square Colour"){
							reference[row][col]->setNormalColour(colour);
						}
						else{
							if(act->text()=="&Solve Colour"){
								reference[row][col]->setSolveColour(colour);
							}
						}
					}
				}
			}
		}
	}
}
