#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

#define MAX_LINE 20
#define MAX_BYTE 75
int textSize = 0, textLine = 0;
string order, orderF, orderS, orderT;
string console = "";

class TextEditor {
private:
	// line은 전체에서 몇번 째 line인지, word는 현재 몇 byte인지를 저장한다.
	int line, word;
	// text는 test.txt 파일을 저장한 리스트 이다
	list<string> text;
	// lineList는 출력하면서 지나간 line을 기록한다
	list<int> lineList;
	// startIter의 경우 출력시 시작하는 부분, endIter는 끝나는 부분을 저장하고,
	// pageIter의 경우 page를 넘어갈 때 이전 startIter를 저장하는 역할을 한다.
	list<list<string>::iterator> pageIter;
	list<string>::iterator startIter;
	list<string>::iterator endIter;

public:
	// 생성자가 불리면 line과 word가 초기화된다.
	TextEditor() {
		line = 0;
		word = 0;
	}
	// 현재 line(몇번 째 line까지 출력되었는지)를 반환한다.
	int getLine() {
		return line;
	}
	void setModel(list<string> _text);
	void showPage();
	void setSize();
	void setOrder(string order);
	void nextPage();
	void previousPage();
	void insert(string order);
	void deleteChar(string order);
	void change(string order);
	void search(string order);
	void terminate();
};

class View {
public:
	// 아래의 print 함수들은 정해진 규격에 맞게 화면에 값을 출력하게 된다.
	void printMenu() {
		cout << "n: 다음페이지, p: 이전페이지, i: 삽입, d: 삭제, c: 변경, s: 찾기, t: 저장후 종료" << endl;
	}

	void print__() {
		cout << "---------------------------------------------------------------------------------" << endl;
	}

	void printUserInput() {
		cout << "입력: ";
	}

	void printConsole(string console) {
		cout << "(콘솔메시지) " << console << endl;
	}
};

class Controller {
private:
	// Controller는 model(textEditor)과 Viewer(View)를 갖는다.
	TextEditor textEditor;
	View view;

public:
	// 생성자를 통해 입력받은 model 과 viewer를 초기화시켜준다.
	Controller(TextEditor _textEditor, View _view) {
		textEditor = _textEditor;
		view = _view;
	}

	// order들과 console의 값을 초기화시켜주는 함수이다.
	void setDefault() {
		console = "";
		orderF = "";
		orderS = "";
		orderT = "";
	}

	// 프로그램이 실행되면 실재로 주 역할을 맞는 함수이다.
	void run() {
		ifstream readFile;
		list<string> textList;

		readFile.open("test.txt");

		// test.txt 파일을 열고 단어 하나하나씩 textList에 저장한다.
		// 만약 파일 읽기를 실패하면 프로그램이 종료된다.
		string word;
		if (!readFile.fail()) {
			while (readFile >> word) {
				textList.push_back(word);
			}

			readFile.close();
			textEditor.setModel(textList);
			textEditor.setSize();

			textEditor.showPage();
			// do_while문을 통해 사용자의 명령을 받고 알맞은 실행을 하게된다.
			// 사용자가 t를 입력하여 저장후 종료를 명령하거나, ctrl-z를 통해 eof를 실행하면 프로그램이 종료된다.
			do {
				view.print__();
				view.printMenu();
				view.print__();
				view.printConsole(console);
				setDefault();
				view.print__();
				view.printUserInput();
				getline(cin, order);
				order.erase(remove(order.begin(), order.end(), ' '), order.end());
				switch (order[0]) {
				case 'n':
					if (textEditor.getLine() >= textLine) {
						console = "This is the last page.";
					}
					else {
						view.print__();
						textEditor.nextPage();
					}
					break;
				case 'p':
					if (textEditor.getLine() <= MAX_LINE) {
						console = "This is the first page.";
					}
					else {
						view.print__();
						textEditor.previousPage();
					}
					break;
				case 'i':
					view.print__();
					textEditor.insert(order);
					break;
				case 'd':
					view.print__();
					textEditor.deleteChar(order);
					break;
				case 'c':
					view.print__();
					textEditor.change(order);
					break;
				case 's':
					view.print__();
					textEditor.search(order);
					break;
				case 't':
					view.print__();
					textEditor.terminate();
					return;
				default:
					console = "존재하지 않는 명령어입니다. 다시 입력해 주십시오.";
				}

			} while (!cin.eof());
		}
		return;
	}

};

// text를 읽은 이후 초기화 가능한 변수들을 초기화 한다.
void TextEditor::setModel(list<string> _text) {
	setSize();
	text = _text;
	startIter = text.begin();
	endIter = text.begin();
}

// 현재 지정된 페이지를 출력하는 기능을 한다.
// 시작 iterator 부터 20line, 75byte의 규격에 맞게 단어를 출력한다.
// 정해진 line까지의 출력을 마치거나, text의 마지막 단어를 출력한 경우 함수를 종료한다.
void TextEditor::showPage() {
	endIter = startIter;
	int printLine = 1, printWord = 0;
	printf("%.2d| ", printLine);
	while (printLine <= MAX_LINE && endIter != text.end()) {
		if (printWord + size(*endIter) > MAX_BYTE) {
			printWord = 0, line++; printLine++;
			if (printLine <= MAX_LINE) {
				printf("\n%.2d| ", printLine);
			}
		}
		else {
			printWord += size(*endIter);
			cout << *endIter;
			if (printWord < MAX_BYTE) {
				printWord++;
				cout << " ";
			}
			endIter++;
		}
	}
	cout << endl;
}


// 다음페이지로 넘길 때 사용하는 함수이다.
// 일반적인 상황에서는 이전페이지의 마지막 출력의 다음 단어를 시작 단어로 설장하고,
// 만약 남은 출력 line이 MAX_LINE(20) 미만이라면 마지막 line이 20번째 줄에 위치하도록 시작단어를 지정한다.
void TextEditor::nextPage() {
	pageIter.push_back(startIter);
	lineList.push_back(line - MAX_LINE);
	if (line + MAX_LINE <= textLine) {
		startIter = endIter;
	}
	else {
		int setWord = 0, setLine = 0;
		while (setLine <= textLine - line) {
			if (setWord + size(*startIter) > MAX_BYTE) {
				setWord = 0, setLine++;
			}
			else {
				setWord += size(*startIter);
				if (setWord < MAX_BYTE) {
					setWord++;
				}
				++startIter;
			}
		}
		line = textLine - MAX_LINE + 1;
	}
	showPage();
}

// 이전페이지로 넘길 때 사용하는 함수이다.
// pageIter에 이전 페이지의 시작주소가 저장되어있는 경우 가져다 쓰고,
// 없는경우 시작할 부분을 찾아서 출력한다.
void TextEditor::previousPage() {
	if (pageIter.size() != 0) {
		startIter = pageIter.back();
		line = lineList.back();
		pageIter.pop_back();
		lineList.pop_back();
	}
	else {
		startIter = text.begin();
		if (line <= 2 * MAX_LINE) {
			line = 0;
		}
		else {
			if (line + MAX_LINE > textLine) {
				line++;
			}
			line = line - 2 * MAX_LINE;
			int setWord = 0, setLine = 0;
			while (setLine < line) {
				if (setWord + size(*startIter) > MAX_BYTE) {
					setWord = 0, setLine++;
				}
				else {
					setWord += size(*startIter);
					if (setWord < MAX_BYTE) {
						setWord++;
					}
					++startIter;
				}
			}
		}
	}
	showPage();
}

// insert의 경우 현재 페이지의 원하는 line, byte에 단어를 삽입하는 함수이다.
// 입력할 위치가 숫자가 아닌경우, MAX_LINE,MAX_BYTE를 넘는경우 함수를 실행하지 않는다.
void TextEditor::insert(string order) {
	setOrder(order);
	for (int i = 0; i < orderF.length(); i++) {
		if (orderF[i] < '0' || orderF[i] > '9') {
			console = "insert의 첫번째 인자는 숫자입니다.";
			return;
		}
	}

	for (int i = 0; i < orderS.length(); i++) {
		if (orderS[i] < '0' || orderS[i] > '9') {
			console = "insert의 두번째 인자는 숫자입니다.";
			return;
		}
	}

	int line = stoi(orderF), byte = stoi(orderS);
	if (line > MAX_LINE) {
		console = "Line은 최대 " + std::to_string(MAX_LINE) + "입니다.";
		return;
	}
	if (byte > MAX_BYTE) {
		console = "Byte은 최대 " + std::to_string(MAX_BYTE) + "입니다.";
		return;
	}
	string insertString = orderT;

	int insertLine = 0, insertWord = 0;
	endIter = startIter;
	while (insertLine < line - 1) {
		if (insertWord + size(*endIter) > MAX_BYTE) {
			insertWord = 0, insertLine++;
		}
		else {
			insertWord += size(*endIter);
			if (insertWord < MAX_BYTE && insertLine != line - 1) {
				insertWord++;
			}
			endIter++;
		}
	}

	while (insertWord + size(*endIter) < byte) {
		insertWord += size(*endIter) + 1;
		endIter++;
	}

	if (insertWord + size(*endIter) > MAX_BYTE) {
		text.insert(endIter, insertString);
	}

	else {
		string newString = *endIter;
		newString.insert(byte - insertWord, insertString);
		text.insert(endIter, newString);
		if (endIter == startIter) {
			startIter--;
		}
		text.erase(endIter);
	}

	setSize();
	int tempLine = line;
	showPage();
	line = tempLine;
}

// deleteChar의 경우 현재 페이지의 원하는 line, byte부터 입력된 byte만큼 삭제하는 함수이다.
// 삭제할 위치가 숫자가 아닌경우, MAX_LINE,MAX_BYTE를 넘는경우 함수를 실행하지 않는다.
void TextEditor::deleteChar(string order) {
	setOrder(order);
	for (int i = 0; i < orderF.length(); i++) {
		if (orderF[i] < '0' || orderF[i] > '9') {
			console = "delete의 두번째 인자는 숫자입니다.";
			return;
		}
	}

	for (int i = 0; i < orderS.length(); i++) {
		if (orderS[i] < '0' || orderS[i] > '9') {
			console = "delete의 두번째 인자는 숫자입니다.";
			return;
		}
	}

	for (int i = 0; i < orderT.length(); i++) {
		if (orderT[i] < '0' || orderT[i] > '9') {
			console = "delete의 세번째 인자는 숫자입니다.";
			return;
		}
	}
	int line = stoi(orderF), byte = stoi(orderS), deleteByte = stoi(orderT);
	if (line > MAX_LINE) {
		console = "Line은 최대 " + std::to_string(MAX_LINE) + "입니다.";
		return;
	}
	if (byte > MAX_BYTE) {
		console = "Byte은 최대 " + std::to_string(MAX_BYTE) + "입니다.";
		return;
	}

	int deleteLine = 0, deleteWord = 0;
	endIter = startIter;
	while (deleteLine < line - 1) {
		if (deleteWord + size(*endIter) > MAX_BYTE) {
			deleteWord = 0, deleteLine++;
		}
		else {
			deleteWord += size(*endIter);
			if (deleteWord < MAX_BYTE && deleteLine != line - 1) {
				deleteWord++;
			}
			endIter++;
		}
	}

	while (deleteWord + size(*endIter) < byte) {
		deleteWord += size(*endIter) + 1;
		endIter++;
	}

	byte = byte - deleteWord;
	string newString;

	while (deleteByte > 0) {
		string tempString = *endIter;
		int tempStrLen = size(tempString);
		if (deleteByte < tempStrLen - byte) {
			tempString = tempString.substr(0, byte) + tempString.substr(byte + deleteByte, tempStrLen);
		}
		else
		{
			tempString = tempString.substr(0, byte);
			if (deleteByte - tempStrLen >= 0) {
				if (endIter == startIter) {
					startIter++;
				}
				text.erase(endIter++);
			}
		}
		deleteByte -= tempStrLen - byte + 1;
		byte = 0;
		newString += tempString;
	}
	if (deleteByte == 0) {
		newString += *endIter;

	}
	if (size(newString) != 0) {
		text.insert(endIter, newString);
		if (endIter == startIter) {
			startIter--;
		}
		text.erase(endIter);
	}
	setSize();
	int tempLine = line;
	showPage();
	line = tempLine;
}

// change의 경우 text에 존재하는 사용자가 입력한 첫번째 단어를
// 입력받은 두번 째 단어로 바꾸는 함수이다.
void TextEditor::change(string order) {
	setOrder(order);
	string before = orderF;
	string after = orderS;
	startIter = text.begin();
	while (startIter != text.end()) {
		if (!(*startIter).compare(before)) {
			text.erase(startIter++);
			text.insert(startIter, after);
		}
		else {
			startIter++;
		}
	}
	setSize();
	startIter = text.begin();
	line = 0;
	showPage();
}

// search의 경우 입력받은 단어를 text에서 찾고, 있을 경우 해당 단어가 맨 처음에 나오게 한다.
// 만약 없을 경우 해당단어가 존재하지 않는다고 console을 출력하고, 첫페이지를 출력한다.
void TextEditor::search(string order) {
	setOrder(order);
	string findString = orderF;
	pageIter.clear();
	lineList.clear();
	startIter = text.begin();
	line = 1;
	bool searchOk = false;
	int searchWord = 1;
	while (startIter != text.end()) {
		if (searchWord + size(*startIter) > MAX_BYTE) {
			searchWord = 0, line++;
		}
		else {
			if (!(*startIter).compare(findString)) {
				if (searchWord + word >= MAX_BYTE) {
					line--;
				}
				searchOk = true;
				break;
			}
			searchWord += size(*startIter);
			if (searchWord < MAX_BYTE) {
				searchWord++;
			}
			++startIter;
		}
	}
	if (!searchOk) {
		console = "해당 단어가 text내에 존재하지 않습니다.";
		startIter = text.begin();
		line = 0;
	}
	showPage();
}

// tertminate는 여러 과정을 거친 text를 저장하고, 프로그램을 종료하는 함수이다.
void TextEditor::terminate() {
	ofstream writeFile;
	writeFile.open("test.txt");
	if (!writeFile.fail()) {
		startIter = text.begin();
		for (; startIter != text.end(); ++startIter) {
			writeFile << *startIter << " ";
		}
		writeFile.close();
	}
}

// setSize는 text의 line, byte를 측정하는 함수이다.
// 처음 text를 입력받을때, text가 수정되었을 때 값이 변경되므로 그때 실행한다.
void TextEditor::setSize() {
	int setLine = 0;
	textSize = 0, textLine = 0;
	for (list<string>::iterator it = text.begin(); it != text.end(); ++it) {
		if (setLine + size(*it) > MAX_BYTE) {
			textLine++;
			setLine = 0;
		}
		setLine += size(*it);
		textSize += size(*it);
		if (setLine < MAX_BYTE) {
			setLine++;
			textSize++;
		}
	}
	word = setLine;
}

// 사용자로부터 정해진 명령어를 입력받게 되면 정해진 규격에 맞게 명령어를 나누는 함수이다.
// 현재 ()안에 최대 3개의 변수가 들어가니 상황에 맞게 3개로 나누게 하였다.
void TextEditor::setOrder(string order) {
	int temp = 2, subStrNum = 0;
	for (int i = 2; i < order.length(); i++) {
		if (order[i] == ',' || order[i] == ')') {
			if (subStrNum == 0) {
				orderF = order.substr(temp, i - temp);
				temp = i + 1;
				subStrNum++;
			}
			else if (subStrNum == 1) {
				orderS = order.substr(temp, i - temp);
				temp = i + 1;
				subStrNum++;
			}
			else {
				orderT = order.substr(temp, i - temp);
				break;
			}
		}
	}
}

// main은 오직 Controller.run()을 통해 Controller를 실행시켜주는 일을 한다.
int main() {
	TextEditor textEditor;
	View view;
	Controller controller(textEditor, view);
	controller.run();
}