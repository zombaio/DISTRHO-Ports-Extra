
dofile("../../../scripts/make-project.lua")

package = make_juce_lv2_project("argotlunar")

package.includepaths = {
  package.includepaths,
  "../Source",
  "../JuceLibraryCode"
}

package.files = {
  matchfiles (
    "../Source/*.cpp",
    "../../../libs/juce-plugin/JucePluginMain.cpp"
  )
}
