//--------------------------------------------------------------------------------
//
// Filename    : resource_validator.cpp
// Written By  : Resource Management Tool
// Description : Resource integrity deep validation tool
//
// 功能:
//   1. 加载 VS_UI_filepath.inf
//   2. 对每个资源文件进行深度验证
//   3. 报告缺失/损坏的资源
//
//--------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdio>

using namespace std;

//--------------------------------------------------------------------------------
// Validation Result Structure
//--------------------------------------------------------------------------------
struct ValidationResult {
	string name;
	string path;
	bool exists;
	bool valid;
	string error;
	string details;
};

vector<ValidationResult> g_Results;

// Base directory for resource files (extracted from .inf file path)
string g_BaseDir;

//--------------------------------------------------------------------------------
// SpritePack forward declaration (minimal interface)
// Stub implementation for standalone validation
//--------------------------------------------------------------------------------
class CSpritePack {
public:
	CSpritePack() : m_Size(0) {}
	~CSpritePack() {}

	bool LoadFromFile(const char* filename);
	int GetSize() const { return m_Size; }

private:
	int m_Size;
};

//--------------------------------------------------------------------------------
// Get full path by combining base directory with resource path
//--------------------------------------------------------------------------------
string GetFullPath(const string& resourcePath) {
	// If resource path is already absolute or starts with base dir, return as-is
	if (resourcePath.empty() || resourcePath[0] == '/' ||
	    resourcePath.find(g_BaseDir) == 0) {
		return resourcePath;
	}

	// Otherwise, prepend base directory
	string fullPath = g_BaseDir;
	if (!fullPath.empty() && fullPath[fullPath.length() - 1] != '/') {
		fullPath += '/';
	}
	fullPath += resourcePath;

	return fullPath;
}

//--------------------------------------------------------------------------------
// Simple file existence check
//--------------------------------------------------------------------------------
bool FileExists(const string& path) {
	ifstream file(path.c_str());
	return file.is_open();
}

//--------------------------------------------------------------------------------
// Simple .inf file validator
//--------------------------------------------------------------------------------
bool ValidateINF(const string& path, ValidationResult& result) {
	ifstream file(path.c_str());
	if (!file.is_open()) {
		result.error = "Cannot open file";
		return false;
	}

	// Count valid entries
	int count = 0;
	string line;
	while (getline(file, line)) {
		// Skip empty lines and comments
		if (line.empty() || line[0] == '#' || line[0] == ' ' || line[0] == '\t')
			continue;

		// Check for separator ':'
		if (line.find(':') != string::npos) {
			count++;
		}
	}
	file.close();

	char buf[256];
	snprintf(buf, sizeof(buf), "Parsed: %d properties", count);
	result.details = buf;

	return true;
}

//--------------------------------------------------------------------------------
// Validate .spk file (minimal check)
//--------------------------------------------------------------------------------
bool ValidateSPK(const string& path, ValidationResult& result) {
	ifstream file(path.c_str(), ios::binary);
	if (!file.is_open()) {
		result.error = "Cannot open file";
		return false;
	}

	// Check file size
	file.seekg(0, ios::end);
	size_t size = file.tellg();
	file.close();

	if (size < 4) {
		result.error = "File too small (< 4 bytes)";
		return false;
	}

	// Check for corresponding index file
	string indexPath = path + "i";
	ifstream indexFile(indexPath.c_str(), ios::binary);
	if (!indexFile.is_open()) {
		result.error = "Missing index file " + indexPath;
		return false;
	}
	indexFile.close();

	char buf[256];
	snprintf(buf, sizeof(buf), "Size: %zu bytes", size);
	result.details = buf;

	return true;
}

//--------------------------------------------------------------------------------
// Get file extension
//--------------------------------------------------------------------------------
string GetExtension(const string& path) {
	size_t pos = path.rfind('.');
	if (pos != string::npos && pos < path.length() - 1) {
		return path.substr(pos);
	}
	return "";
}

//--------------------------------------------------------------------------------
// Validate single resource
//--------------------------------------------------------------------------------
void ValidateResource(const string& name, const string& path) {
	ValidationResult result;
	result.name = name;
	result.path = path;
	result.exists = false;
	result.valid = false;

	// Skip directory paths
	if (!path.empty() && path[path.length() - 1] == '/') {
		result.valid = true;
		result.exists = true;
		result.details = "Directory path";
		g_Results.push_back(result);
		return;
	}

	// Get full path by combining base directory with resource path
	string fullPath = GetFullPath(path);

	// Check file existence
	ifstream file(fullPath.c_str());
	if (!file.is_open()) {
		result.error = "File not found";
		g_Results.push_back(result);
		return;
	}
	file.close();
	result.exists = true;

	// Deep validation based on extension
	string ext = GetExtension(path);

	if (ext == ".spk") {
		result.valid = ValidateSPK(fullPath, result);
	} else if (ext == ".inf") {
		result.valid = ValidateINF(fullPath, result);
	} else if (ext == ".ispk" || ext == ".cfpk" || ext == ".sspk" ||
	           ext == ".rpk" || ext == ".txt" || ext == ".xml" ||
	           ext == ".spki" || ext == ".ispki") {
		// Other resource files - existence check only
		result.valid = true;
		result.details = "File exists (no deep validation)";
	} else {
		// Unknown type
		result.valid = true;
		result.details = "Unknown type - exists only";
	}

	g_Results.push_back(result);
}

//--------------------------------------------------------------------------------
// Parse .inf file manually (key:value format)
//--------------------------------------------------------------------------------
map<string, string> ParseInfFile(const string& filename) {
	map<string, string> properties;
	ifstream file(filename.c_str());

	if (!file.is_open()) {
		cerr << "Warning: Cannot open " << filename << endl;
		return properties;
	}

	string line;
	while (getline(file, line)) {
		// Remove trailing \r (Windows CRLF files on Unix/macOS)
		if (!line.empty() && line[line.size() - 1] == '\r') {
			line.erase(line.size() - 1);
		}

		// Skip empty lines and comments
		if (line.empty() || line[0] == '#' || line[0] == ' ' || line[0] == '\t')
			continue;

		// Find separator ':'
		size_t sep = line.find(':');
		if (sep == string::npos)
			continue;

		// Extract key and value
		string key = line.substr(0, sep);
		string value = line.substr(sep + 1);

		// Trim whitespace
		size_t key_end = key.find_last_not_of(" \t");
		if (key_end != string::npos) {
			key = key.substr(0, key_end + 1);
		}

		size_t value_begin = value.find_first_not_of(" \t");
		if (value_begin != string::npos) {
			value = value.substr(value_begin);
		}

		properties[key] = value;
	}

	file.close();
	return properties;
}

//--------------------------------------------------------------------------------
// Print validation report
//--------------------------------------------------------------------------------
void PrintReport() {
	cout << "\n========================================" << endl;
	cout << "    Resource Validation Report" << endl;
	cout << "========================================" << endl;
	cout << "Total: " << g_Results.size() << " resources" << endl;

	int valid = 0, invalid = 0, missing = 0, skipped = 0;
	for (size_t i = 0; i < g_Results.size(); i++) {
		const ValidationResult& r = g_Results[i];
		if (!r.exists) missing++;
		else if (!r.valid) invalid++;
		else if (r.details.find("Directory") != string::npos) skipped++;
		else valid++;
	}

	cout << "\nStatistics:" << endl;
	cout << "  Valid:       " << valid << endl;
	cout << "  Corrupted:   " << invalid << endl;
	cout << "  Missing:     " << missing << endl;
	cout << "  Skipped:     " << skipped << " (directories)" << endl;

	// Print problem resources
	if (invalid > 0 || missing > 0) {
		cout << "\n========================================" << endl;
		cout << "    Problem Resources" << endl;
		cout << "========================================" << endl;

		for (size_t i = 0; i < g_Results.size(); i++) {
			const ValidationResult& r = g_Results[i];
			if (!r.exists || !r.valid) {
				cout << "\n  " << r.name << endl;
				cout << "    Path:   " << r.path << endl;
				cout << "    Status: ";
				if (!r.exists) {
					cout << "File not found" << endl;
				} else {
					cout << r.error << endl;
				}
			}
		}
	}

	// Print valid resources sample (first 10)
	if (valid > 0) {
		cout << "\n========================================" << endl;
		cout << "    Valid Resources (Sample)" << endl;
		cout << "========================================" << endl;

		int count = 0;
		for (size_t i = 0; i < g_Results.size(); i++) {
			const ValidationResult& r = g_Results[i];
			if (r.valid && r.exists &&
			    r.details.find("Directory") == string::npos) {
				cout << "  " << r.name << " -> " << r.path;
				if (!r.details.empty()) {
					cout << " (" << r.details << ")";
				}
				cout << endl;

				if (++count >= 10) break;
			}
		}
		if (valid > 10) {
			cout << "  ... and " << (valid - 10) << " more" << endl;
		}
	}

	cout << "\n========================================" << endl;

	// Return code
	int exitCode = (invalid == 0 && missing == 0) ? 0 : 1;
	if (exitCode == 0) {
		cout << "Result: All resources are valid!" << endl;
	} else {
		cout << "Result: Validation failed!" << endl;
	}
	cout << "========================================\n" << endl;
}

//--------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
	const char* infFile = "DarkEden/Data/Info/VS_UI_filepath.inf";

	if (argc > 1) {
		infFile = argv[1];
	}

	cout << "Resource Validator" << endl;
	cout << "==================" << endl;
	cout << "Loading resource list: " << infFile << endl;

	// Extract base directory from .inf file path
	// If path is "/path/to/DarkEden/Data/Info/VS_UI_filepath.inf"
	// We want to extract "/path/to/DarkEden"
	string infPath = infFile;
	size_t dataInfoPos = infPath.find("/Data/Info/VS_UI_filepath.inf");
	if (dataInfoPos != string::npos) {
		// Found "/Data/Info/VS_UI_filepath.inf", extract base dir
		g_BaseDir = infPath.substr(0, dataInfoPos);
	} else {
		// Try alternative pattern: "DarkEden/Data/Info/"
		size_t darkEdenPos = infPath.find("DarkEden/Data/Info/");
		if (darkEdenPos != string::npos) {
			g_BaseDir = infPath.substr(0, darkEdenPos + 8); // Include "DarkEden"
		} else {
			// No pattern found, use current directory
			g_BaseDir = ".";
		}
	}

	cout << "Base directory: " << g_BaseDir << endl;

	// Parse resource list manually
	map<string, string> resources = ParseInfFile(infFile);

	if (resources.empty()) {
		cout << "Error: Failed to load resource list or file is empty" << endl;
		return 1;
	}

	cout << "Loaded " << resources.size() << " resource definitions" << endl;
	cout << "Validating resources..." << endl;

	// Validate each resource
	for (map<string, string>::const_iterator it = resources.begin();
	     it != resources.end(); ++it) {
		ValidateResource(it->first, it->second);
	}

	// Print report
	PrintReport();

	// Exit code: 0 = all valid, 1 = some problems
	int invalid = 0, missing = 0;
	for (size_t i = 0; i < g_Results.size(); i++) {
		const ValidationResult& r = g_Results[i];
		if (!r.exists) missing++;
		else if (!r.valid) invalid++;
	}

	return (invalid == 0 && missing == 0) ? 0 : 1;
}

//--------------------------------------------------------------------------------
// Stub implementation for CSpritePack::LoadFromFile
// In real implementation, this would call the actual SpriteLib loader
//--------------------------------------------------------------------------------
bool CSpritePack::LoadFromFile(const char* filename) {
	// For minimal validation, just check file exists and has reasonable size
	ifstream file(filename, ios::binary);
	if (!file.is_open()) return false;

	file.seekg(0, ios::end);
	size_t size = file.tellg();
	file.close();

	return size >= 4;  // Minimum size check
}
