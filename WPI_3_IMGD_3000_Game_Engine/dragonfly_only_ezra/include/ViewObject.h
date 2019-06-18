#ifndef __VIEW_OBJECT_H__
#define __VIEW_OBJECT_H__

#include "Object.h"
#include "Event.h"

namespace df {
// General location of ViewObject on screen.
enum ViewObjectLocation {
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
	CENTER_CENTER,
	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT,
};

class ViewObject : public Object {
private:
	std::string view_string;	// Label for value (e.g. "Points").
	int value;					// Value displayed (e.g., points).
	bool border;				// True if border around display.
	Color color;				// Color for text.
	ViewObjectLocation location;// Location of ViewObject.

public:
	// Construct ViewObject.
	// Object settings: SPECTRAL, max_alt.
	// ViewObject defaults: border, top_center, default color.
	ViewObject();

	// Draw view string and value.
	virtual void draw(void);

	// Handle 'view' event if tag matches view_string (others ignored).
	// return 0 if ignored, else 1 if handled.
	virtual int eventHandler(const Event *p_e);

	// General location of ViewObject on screen.
	void setLocation(ViewObjectLocation new_loc);
	ViewObjectLocation getLocation() const;

	// Set/get view value.
	void setValue(int new_value);
	int getValue() const;

	// Set/get view border (true = display border)
	void setBorder(bool new_border);
	bool getBorder() const;

	// Get/set view color
	void setColor(Color c);
	Color getColor() const;

	// Set/Get view display string.
	void setViewString(std::string s);
	std::string getViewString() const;
};

}
#endif
