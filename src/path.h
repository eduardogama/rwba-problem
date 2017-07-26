#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <vector>

using namespace std;

class Path {

public:
	Path(unsigned maxLength = 0);
	~Path(){}

	unsigned  getFrom() const {
		return from;
	}
	unsigned  getLength() const {
		return length;
	}
	unsigned  getTo() const {
		return to;
	}
	void addLinkToPath(unsigned next);
	void goStart(void);
	unsigned getActualStep(void);
	unsigned getNextStep(void);
	void goAhead(void);
	void goLastLink(void);
	bool isEndPath(void) const;
	void clear();

	friend std::ostream &operator<<(std::ostream &os, const Path &path);

private:
	unsigned length;
	//from e to representam o no
	unsigned from;
	unsigned to;
	//actualPos representa o indice no vector
	unsigned actualPos;
	vector<unsigned> path;

};

#endif // PATH_H
