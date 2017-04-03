/*
CMS50D+ Embedded Python Script

cms50dplus.py

Link using libpython and libboost_python

https://wiki.python.org/moin/boost.python/EmbeddingPython
*/


boost::python::object
myapp::python::import( const myapp::Path & s )
{
  using namespace boost::python;
  try
  {
    // If path is /Users/whatever/blah/foo.py
    dict locals;
    locals["cms50dplus.py"] = s.filebase(); // foo -> module name
    locals["/O2Controller/Sensors/cms50dplus.py"]   = s.get(); // /Users/whatever/blah/foo.py
    exec("import imp\n"
         "newmodule = imp.load_module(modulename,open(path),path,('py','U',imp.PY_SOURCE))\n",
         globals(),locals);
    return locals["newmodule"];
  }
  catch( error_already_set )
  {
    error(currentException());
    return object();
  }
}



#include <boost/python.hpp>

using namespace boost::python;

int main( int argc, char ** argv ) {
  try {
    Py_Initialize();

    object main_module((
      handle<>(borrowed(PyImport_AddModule("__main__")))));

    object main_namespace = main_module.attr("__dict__");

    handle<> ignored(( PyRun_String( "print \"Hello, World\"",
                                     Py_file_input,
                                     main_namespace.ptr(),
                                     main_namespace.ptr() ) ));
  } catch( error_already_set ) {
    PyErr_Print();
  }
}


