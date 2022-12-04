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
	// line�� ��ü���� ��� ° line����, word�� ���� �� byte������ �����Ѵ�.
	int line, word;
	// text�� test.txt ������ ������ ����Ʈ �̴�
	list<string> text;
	// lineList�� ����ϸ鼭 ������ line�� ����Ѵ�
	list<int> lineList;
	// startIter�� ��� ��½� �����ϴ� �κ�, endIter�� ������ �κ��� �����ϰ�,
	// pageIter�� ��� page�� �Ѿ �� ���� startIter�� �����ϴ� ������ �Ѵ�.
	list<list<string>::iterator> pageIter;
	list<string>::iterator startIter;
	list<string>::iterator endIter;

public:
	// �����ڰ� �Ҹ��� line�� word�� �ʱ�ȭ�ȴ�.
	TextEditor() {
		line = 0;
		word = 0;
	}
	// ���� line(��� ° line���� ��µǾ�����)�� ��ȯ�Ѵ�.
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
	// �Ʒ��� print �Լ����� ������ �԰ݿ� �°� ȭ�鿡 ���� ����ϰ� �ȴ�.
	void printMenu() {
		cout << "n: ����������, p: ����������, i: ����, d: ����, c: ����, s: ã��, t: ������ ����" << endl;
	}

	void print__() {
		cout << "---------------------------------------------------------------------------------" << endl;
	}

	void printUserInput() {
		cout << "�Է�: ";
	}

	void printConsole(string console) {
		cout << "(�ָܼ޽���) " << console << endl;
	}
};

class Controller {
private:
	// Controller�� model(textEditor)�� Viewer(View)�� ���´�.
	TextEditor textEditor;
	View view;

public:
	// �����ڸ� ���� �Է¹��� model �� viewer�� �ʱ�ȭ�����ش�.
	Controller(TextEditor _textEditor, View _view) {
		textEditor = _textEditor;
		view = _view;
	}

	// order��� console�� ���� �ʱ�ȭ�����ִ� �Լ��̴�.
	void setDefault() {
		console = "";
		orderF = "";
		orderS = "";
		orderT = "";
	}

	// ���α׷��� ����Ǹ� ����� �� ������ �´� �Լ��̴�.
	void run() {
		ifstream readFile;
		list<string> textList;

		readFile.open("test.txt");

		// test.txt ������ ���� �ܾ� �ϳ��ϳ��� textList�� �����Ѵ�.
		// ���� ���� �б⸦ �����ϸ� ���α׷��� ����ȴ�.
		string word;
		if (!readFile.fail()) {
			while (readFile >> word) {
				textList.push_back(word);
			}

			readFile.close();
			textEditor.setModel(textList);
			textEditor.setSize();

			textEditor.showPage();
			// do_while���� ���� ������� ����� �ް� �˸��� ������ �ϰԵȴ�.
			// ����ڰ� t�� �Է��Ͽ� ������ ���Ḧ ����ϰų�, ctrl-z�� ���� eof�� �����ϸ� ���α׷��� ����ȴ�.
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
					console = "�������� �ʴ� ��ɾ��Դϴ�. �ٽ� �Է��� �ֽʽÿ�.";
				}

			} while (!cin.eof());
		}
		return;
	}

};

// text�� ���� ���� �ʱ�ȭ ������ �������� �ʱ�ȭ �Ѵ�.
void TextEditor::setModel(list<string> _text) {
	setSize();
	text = _text;
	startIter = text.begin();
	endIter = text.begin();
}

// ���� ������ �������� ����ϴ� ����� �Ѵ�.
// ���� iterator ���� 20line, 75byte�� �԰ݿ� �°� �ܾ ����Ѵ�.
// ������ line������ ����� ��ġ�ų�, text�� ������ �ܾ ����� ��� �Լ��� �����Ѵ�.
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


// ������������ �ѱ� �� ����ϴ� �Լ��̴�.
// �Ϲ����� ��Ȳ������ ������������ ������ ����� ���� �ܾ ���� �ܾ�� �����ϰ�,
// ���� ���� ��� line�� MAX_LINE(20) �̸��̶�� ������ line�� 20��° �ٿ� ��ġ�ϵ��� ���۴ܾ �����Ѵ�.
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

// ������������ �ѱ� �� ����ϴ� �Լ��̴�.
// pageIter�� ���� �������� �����ּҰ� ����Ǿ��ִ� ��� ������ ����,
// ���°�� ������ �κ��� ã�Ƽ� ����Ѵ�.
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

// insert�� ��� ���� �������� ���ϴ� line, byte�� �ܾ �����ϴ� �Լ��̴�.
// �Է��� ��ġ�� ���ڰ� �ƴѰ��, MAX_LINE,MAX_BYTE�� �Ѵ°�� �Լ��� �������� �ʴ´�.
void TextEditor::insert(string order) {
	setOrder(order);
	for (int i = 0; i < orderF.length(); i++) {
		if (orderF[i] < '0' || orderF[i] > '9') {
			console = "insert�� ù��° ���ڴ� �����Դϴ�.";
			return;
		}
	}

	for (int i = 0; i < orderS.length(); i++) {
		if (orderS[i] < '0' || orderS[i] > '9') {
			console = "insert�� �ι�° ���ڴ� �����Դϴ�.";
			return;
		}
	}

	int line = stoi(orderF), byte = stoi(orderS);
	if (line > MAX_LINE) {
		console = "Line�� �ִ� " + std::to_string(MAX_LINE) + "�Դϴ�.";
		return;
	}
	if (byte > MAX_BYTE) {
		console = "Byte�� �ִ� " + std::to_string(MAX_BYTE) + "�Դϴ�.";
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

// deleteChar�� ��� ���� �������� ���ϴ� line, byte���� �Էµ� byte��ŭ �����ϴ� �Լ��̴�.
// ������ ��ġ�� ���ڰ� �ƴѰ��, MAX_LINE,MAX_BYTE�� �Ѵ°�� �Լ��� �������� �ʴ´�.
void TextEditor::deleteChar(string order) {
	setOrder(order);
	for (int i = 0; i < orderF.length(); i++) {
		if (orderF[i] < '0' || orderF[i] > '9') {
			console = "delete�� �ι�° ���ڴ� �����Դϴ�.";
			return;
		}
	}

	for (int i = 0; i < orderS.length(); i++) {
		if (orderS[i] < '0' || orderS[i] > '9') {
			console = "delete�� �ι�° ���ڴ� �����Դϴ�.";
			return;
		}
	}

	for (int i = 0; i < orderT.length(); i++) {
		if (orderT[i] < '0' || orderT[i] > '9') {
			console = "delete�� ����° ���ڴ� �����Դϴ�.";
			return;
		}
	}
	int line = stoi(orderF), byte = stoi(orderS), deleteByte = stoi(orderT);
	if (line > MAX_LINE) {
		console = "Line�� �ִ� " + std::to_string(MAX_LINE) + "�Դϴ�.";
		return;
	}
	if (byte > MAX_BYTE) {
		console = "Byte�� �ִ� " + std::to_string(MAX_BYTE) + "�Դϴ�.";
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

// change�� ��� text�� �����ϴ� ����ڰ� �Է��� ù��° �ܾ
// �Է¹��� �ι� ° �ܾ�� �ٲٴ� �Լ��̴�.
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

// search�� ��� �Է¹��� �ܾ text���� ã��, ���� ��� �ش� �ܾ �� ó���� ������ �Ѵ�.
// ���� ���� ��� �ش�ܾ �������� �ʴ´ٰ� console�� ����ϰ�, ù�������� ����Ѵ�.
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
		console = "�ش� �ܾ text���� �������� �ʽ��ϴ�.";
		startIter = text.begin();
		line = 0;
	}
	showPage();
}

// tertminate�� ���� ������ ��ģ text�� �����ϰ�, ���α׷��� �����ϴ� �Լ��̴�.
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

// setSize�� text�� line, byte�� �����ϴ� �Լ��̴�.
// ó�� text�� �Է¹�����, text�� �����Ǿ��� �� ���� ����ǹǷ� �׶� �����Ѵ�.
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

// ����ڷκ��� ������ ��ɾ �Է¹ް� �Ǹ� ������ �԰ݿ� �°� ��ɾ ������ �Լ��̴�.
// ���� ()�ȿ� �ִ� 3���� ������ ���� ��Ȳ�� �°� 3���� ������ �Ͽ���.
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

// main�� ���� Controller.run()�� ���� Controller�� ��������ִ� ���� �Ѵ�.
int main() {
	TextEditor textEditor;
	View view;
	Controller controller(textEditor, view);
	controller.run();
}