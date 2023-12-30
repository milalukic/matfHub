#include "../catch.hpp"
#include "../src/filemanager.cpp"

TEST_CASE("File manager", "[class]"){
	SECTION("Provera praznog back steka"){
		FileManager *fm = FileManager::FileManager(mw);
		fm->currPath.push("123/123/123");
		QString expected = fm->currPath;
		
		fm->backButtonClicked();
		
		REQUIRE(fm->currPath == expected);
	}
	
	SECTION("Provera rada back steka"){
		FileManager *fm = FileManager::FileManager(mw);
	}
}
