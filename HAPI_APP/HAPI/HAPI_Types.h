#pragma once
/*
	Aliases, enums and structs used with HAPI
*/

namespace HAPISPACE {

	/** <summary> an alias for a type used to hold a byte (1 byte or 8 bits) of data </summary> */
	using BYTE = unsigned char;

	/** <summary> an alias for a type used to hold an unsigned WORD (2 bytes or 16 bits) of data </summary> */
	using WORD = unsigned short;

	/** <summary> an alias for a type used to hold a signed WORD (2 bytes or 16 bits) of data </summary> */
	using SWORD = signed short;

	/** <summary>	an alias for a type used to hold two WORDS (4 bytes or 32 bits) of data. </summary> */
	using DWORD = unsigned long;

	/** <summary>	Flag values that can be used during initialise. </summary> */
	enum HAPI_InitFlags
	{
		eDefaults = 0,
		eFullscreen = 1 << 0,
		ePerfOverlay = 1 << 1
	};

	/** <summary>	Supported image foramts. </summary> */
	enum class HAPI_ImageFileFormat
	{
		eBMP,
		ePCX,
		ePNG,
		eTGA,
		eJPG
		// Note: could easily add some more if requested
	};

	/**
	 * <summary>
	 * Text format bit flags that can be combined e.g. HAPISPACE::eBold & HAPISPACE::eItalic.
	 * </summary>
	 */
	enum HAPI_TextStyle
	{
		eRegular = 0,			// Regular characters, no style
		eBold = 1 << 0,			// Bold characters
		eItalic = 1 << 1,		// Italic characters
		eUnderlined = 1 << 2,	// Underlined characters
		eStrikeThrough = 1 << 3	// Strike through characters
	};

	/** <summary>	HAPI Error codes. </summary> */
	enum class HAPI_ErrorCodes
	{
		H_OK = 0,
		H_INITIALISATION_ERROR,
		H_INVALID_PARAMETER,
		H_OUT_OF_MEMORY,
		H_FILE_NOT_FOUND,
		H_INTERNAL_ERROR,
		H_UNSUPPORTED,
		H_DEVICE_NOT_CONNECTED,
		H_NUM_HAPI_ERROR_CODES
	};

	/** <summary>	HAPI systems as specified in errors. </summary> */
	enum class HAPI_System
	{
		H_CORE_SYSTEM = 0,
		H_OS_SYSTEM,
		H_RENDERER_SYSTEM,
		H_SOUND_SYSTEM,
		H_INPUT_SYSTEM,
		H_INTERNAL_SYSTEM,
		H_NUM_HAPI_SYSTEMS
	};

	/** <summary>	Message box button types for use in the HAPI_MessageBox call. </summary> */
	enum class HAPI_ButtonType
	{
		eButtonTypeOk,				// Just an OK button
		eButtonTypeOkCancel,		// OK and CANCEL buttons
		eButtonTypeYesNo,			// YES and NO buttons
		eButtonTypeYesNoCancel		// YES, NO and CANCEL buttons
	};

	/** <summary>	Message box return types from the HAPI_UserMessage call and file dialog call. </summary> */
	enum class HAPI_UserResponse
	{
		eUserResponseUnknown = -1,
		eUserResponseOk,		// OK button clicked by user
		eUserResponseCancel,	// CANCEL button clicked
		eUserResponseYes,		// YES button clicked
		eUserResponseNo			// NO button clicked
	};

	/**
	* \struct HAPI_TMouseData
	* \brief Contains information about the current mouse state
	*/
	struct HAPI_TMouseData final
	{
		unsigned int x{ 0 };
		unsigned int y{ 0 };
		int wheelMovement{ 0 };
		bool leftButtonDown{ false };
		bool rightButtonDown{ false };
		bool middleButtonDown{ false };
		bool xButton1Down{ false };
		bool xButton2Down{ false };
	};

	/**
	* \struct HAPI_TKeyboardData
	* \brief Contains information about the current keyboard state, true if key down false if not
	* Note: you can use the defines in HAPI_InputCodes.h as indices into the  scanCode array
	*/
	struct HAPI_TKeyboardData final
	{
		bool scanCode[256]{};
	};

	// The maximum number of buttons a controller may have
	// Note: use this define rather than hard coding the number as it may change in the future
	#define HAPI_MAX_BUTTON_ARRAY_SIZE	16

	/**
	* \struct HAPI_TControllerData
	* \brief Contains information about a controller state
	* Note that not all controllers have HAPI_MAX_BUTTON_ARRAY_SIZE of each type of button
	* Note: you can use the defines in HAPI_InputCodes.h as indices into the arrays
	*/
	struct HAPI_TControllerData final
	{
		bool digitalButtons[HAPI_MAX_BUTTON_ARRAY_SIZE]{};	// On or off
		int analogueButtons[HAPI_MAX_BUTTON_ARRAY_SIZE]{};	// Value range is -32768 to +32767
		bool isAttached{ false }; // only if this is true are the above values valid - ADDED IN 2.00
	};

	/** <summary>	Used by HAPI to represent a colour. Values range from 0 to 255 per channel. </summary> */
	struct HAPI_TColour final
	{
		BYTE red{ 255 };
		BYTE green{ 255 };
		BYTE blue{ 255 };
		BYTE alpha{ 255 };

		// Constructor 1 - Colour defaults to white	
		HAPI_TColour() = default;

		// Constructor 2 - Colours passed in, alpha defaults to 255 (opaque) if not provided
		HAPI_TColour(BYTE r, BYTE g, BYTE b, BYTE a = 255) : red(r), green(g), blue(b), alpha(a) {}

		// Constructor 3 - gray
		HAPI_TColour(BYTE gray, BYTE a = 255) : red(gray), green(gray), blue(gray), alpha(a) {}
	};

	/**
	 * <summary>
	 * used by HAPI to describe a sound's data make up Note: all HAPI loaded sounds have a bit depth
	 * of 16 (two bytes or a SWORD)
	 * </summary>
	 */
	struct HAPI_TSoundFormat final
	{
		unsigned int numChannels{ 0 };	// 1 is mono, 2 is stereo etc.
		unsigned int numSamples{ 0 };	// How many samples in total
		unsigned int sampleRate{ 0 };	// How many samples per second at normal play back speed
		// Length in seconds helper calculation
		float Seconds() const { return (numChannels * numSamples) / (float)sampleRate; }
	};

	/** <summary>	Structure used to set sound playback options. </summary> */
	struct HAPI_TSoundOptions final
	{
		float pan{ 0 };				// Left is -1 right is 1. Note: if you provide a 3D position, this is ignored
		float volume{ 1.0f };			// from 0 - 1 (full)
		float pitch{ 1.0f };		// 2 would double the pitch and make it play in half the time
		
		float pos3Dx{ 0 };			// If any of these 3 are set to !=0 then these are used to position the sound and pan
		float pos3Dy{ 0 };			//	is ignored
		float pos3Dz{ 0 };			//	Note: 3D positioning only works for mono sounds

		bool loop{ false };			// Should the sound loop

		// Constructor with defaults with most common settings first
		HAPI_TSoundOptions(float volumeI = 1.0f, bool loopI = false, float panI = 0, float pitchI = 1.0f) :
			volume(volumeI), loop(loopI), pan(panI), pitch(pitchI) {}
	};

	/** <summary>	Values that represent the media status. </summary> */
	enum class HAPI_MediaStatus
	{
		eError,		// An error occurred		
		eStarting,	// About to start
		eStopped,	// Stopped and sitting at start / first frame
		ePaused,	// Paused and sitting at a frame
		ePlaying	// Playing
	};

	/** <summary>	Values that represent how to size video. </summary> */
	enum class HAPI_VideoSizing
	{
		eUseOriginalSize,		// Takes the size from the video (but will scale if bigger than window)
		eUseRectangle,			// Takes the size from the rectangle provided in HAPI_TStreamedMediaOptions
		eScaleToWindow			// Scales the video to the size of the window
	};

	/** <summary>	Structure used to set movie and music playback options. </summary> */
	//struct HAPI_TStreamedMediaOptions final
	//{
	//	float volume{ 1.0f };		// From 0 to 1
	//	float startTimeOffset{ 0 };	// Allows video or music to be started part way through				
	//	unsigned int sx{ 0 };		// Left position of video window, depends on sizing 
	//	unsigned int sy{ 0 };		// Top position of video window, depends on sizing
	//	unsigned int width{ 0 };	// Width of video window , depends on sizing
	//	unsigned int height{ 0 };	// Height of video window, depends on sizing
	//	bool loop{ false };			// When media gets to end it is restarted from the beginning if true
	//	bool preserveRatio{ false };// If true width and height ratio is kept whatever the requested resize
	//	HAPI_VideoSizing sizing{ HAPI_VideoSizing::eUseOriginalSize };
	//};

	/** <summary>	Structure used to set movie and music playback options. </summary> */
	struct HAPI_TStreamedMediaOptions
	{
		float volume{ 1.0f };		// From 0 to 1
		float startTimeOffset{ 0 };	// Allows video or music to be started part way through
		bool loop{ false };			// When media gets to end it is restarted from the beginning if true
		HAPI_VideoSizing sizing{ HAPI_VideoSizing::eUseOriginalSize };
		unsigned int sx{ 0 };		// Left position of video window, depends on sizing 
		unsigned int sy{ 0 };		// Top position of video window, depends on sizing
		unsigned int width{ 0 };	// Width of video window , depends on sizing
		unsigned int height{ 0 };	// Height of video window, depends on sizing
		bool preserveRatio{ false };// If true width and height ratio is kept whatever the requested resize
	};

	/** <summary>	Structure used to return information about media. </summary> */
	struct HAPI_TStreamedMediaInfo final
	{
		HAPI_MediaStatus status{ HAPI_MediaStatus::eError }; // See HAPI_StreamedMediaStatus enum
		unsigned int numAudioStreams{ 0 };	// Normally 1 but some Video can have multiple audi streams
		unsigned int numVideoStreams{ 0 };  // Normally 1
		unsigned int numSubtitleStreams{ 0 }; // Normally 0 but some Video can subtitles
		unsigned int audioChannels{ 0 }; // 1 for mono, 2 for stereo
		unsigned int audioSamplingRate{ 0 }; // Samples per second. E.g. 44100 is CD format quality
		bool loop{ false };	// Looping or not
		float duration{ 0 }; // Total duration in seconds
		float frameRate{ 0 };	// Video frames per second
		float currentPosition{ 0 };	// Current playback position in seconds
	};

	// Used to specify filters when calling FileDialog
	struct HAPI_TFileDialogFilters final
	{
		std::string filterName;	// e.g. Bitmap Files
		std::string filter;		// e.g. *.bmp
	};

	// Types of XML node
	enum class HAPI_XMLNodeType
	{
		eNull,
		eDocument,
		eElement,
		ePcdata,
		eCdata,
		eComment,
		ePi,
		eDeclaration
	};

	// XML attribute
	class CHapiXMLAttribute
	{
	private:
		std::string m_name;
		std::string m_stringValue;
	public:
		CHapiXMLAttribute(const std::string &name, const std::string &stringValue) : m_name(name), m_stringValue(stringValue) {}
		CHapiXMLAttribute() = default;

		std::string AsString() const { return m_stringValue; }
		int AsInt() const { return stoi(m_stringValue); }
		float AsFloat() const { return stof(m_stringValue); }
		double AsDouble() const { return stod(m_stringValue); }

		const std::string& GetName() const { return m_name; }
	};

	// An XML node 
	class CHapiXMLNode
	{
	private:
		std::string m_name;
		HAPI_XMLNodeType m_type;
		std::string m_value;
		std::vector<CHapiXMLAttribute> m_attributes;
		std::vector<CHapiXMLNode*> m_children;
		CHapiXMLNode *m_parent{ nullptr };
	public:
		CHapiXMLNode(const std::string& name, HAPI_XMLNodeType type = HAPI_XMLNodeType::eElement, std::string value = std::string(),
			CHapiXMLNode *parent=nullptr) : m_name(name), m_type(type), m_value(value), m_parent(parent) {}

		void AddAttribute(CHapiXMLAttribute &attribute) {m_attributes.push_back(attribute);}
		void AddChild(CHapiXMLNode* child) { child->m_parent = this; m_children.push_back(child); }

		const std::string& GetName() const { return m_name; }
		HAPI_XMLNodeType GetType() const { return m_type; }
		const std::string& GetValue() const { return m_value; }
		const std::vector<CHapiXMLAttribute>& GetAttributes() const { return m_attributes; }
		const std::vector<CHapiXMLNode*>& GetChildren() const { return m_children; }
		CHapiXMLNode * GetParent() const { return m_parent; }

		//  Returns false if does not exist
		bool GetAttributeWithName(const std::string &name, CHapiXMLAttribute& attribute) const;
	};

	class CXMLPimpl;
	
	// Used for working with XML files
	class CHapiXML
	{
	private:
		CXMLPimpl *m_pimpl{ nullptr };
	public:
		// Constructor that loads XML file
		CHapiXML(const std::string &filename);
		// Constructor that creates an XML tree
		CHapiXML(CHapiXMLNode *rootNode);
		~CHapiXML();

		// Used to determine if we have loaded ok
		bool HasData() const { return (m_pimpl != nullptr); }

		// Saves tree as a new XML file, returns false on error
		bool Save(const std::string &filename);

		// Outputs the loaded XML file to the Output pane showing all nodes and attributes
		void DebugOutputStructure() const;

		// Finds and returns the first node in the tree called 'name'. Returns nullptr if it does not exist.
		CHapiXMLNode* GetFirstNodeWithName(const std::string& name) const;

		// Returns a vector of all nodes called 'name' and their attributes
		std::vector<CHapiXMLNode*> GetAllNodesWithName(const std::string& name) const;
		
		// Returns the root of the tree
		CHapiXMLNode* GetRootNode() const;
	};
}