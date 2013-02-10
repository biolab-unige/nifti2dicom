require 'formula'

# Documentation: https://github.com/mxcl/homebrew/wiki/Formula-Cookbook
# PLEASE REMOVE ALL GENERATED COMMENTS BEFORE SUBMITTING YOUR PULL REQUEST!

class Nifti2dicomMacos < Formula
  homepage 'http://www.bio.dist.unige.it/projects/national/85'
  url 'git://github.com/biolab-unige/nifti2dicom.git', :using => :git
  head 'git://github.com/biolab-unige/nifti2dicom.git', :using => :git
  version '0.4'
  sha1 '746fd3674fa52fe1d320780bad221bcb74954b93'

  depends_on 'cmake' => :build
  depends_on 'qt' => :build
  depends_on 'vtk' => [:build, 'qt']
  depends_on 'insighttoolkit' => :build
  depends_on 'tclap'	=> :build
  depends_on 'doxygen' => :build
  #depends_on :x11 # if your formula requires any X11/XQuartz components

  def install
    # ENV.j1  # if your formula's build system can't parallelize
    system "cmake", ".", *std_cmake_args
    system "make & make install"
  end

  def test
    # This test will fail and we won't accept that! It's enough to just replace
    # "false" with the main program this formula installs, but it'd be nice if you
    # were more thorough. Run the test with `brew test nifti2dicom_macos`.
    system "false"
  end
end
