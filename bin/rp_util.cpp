#include "rp_util.hpp"
#include <fstream>

bool processArgs(int argc, char **argv, std::string &pbFilename, std::string &solFilename,
                 std::string &paramFilename, std::string &logFilename,
                 std::vector<std::string> &options, realpaver::Interval &objBounds,
                 std::string &msgError)
{
   int i = 1;
   bool ok = true;

   pbFilename = "";
   paramFilename = "";
   solFilename = "";
   logFilename = "";
   options.clear();

   while (i < argc && ok)
   {
      std::string text(argv[i]);

      if (text.length() > 2 && text.substr(0, 2) == "-P")
      {
         if (paramFilename != "")
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
         else
         {
            std::string str = text.substr(2, text.length() - 2);
            ok = expandTildeInPath(str, paramFilename, msgError);

            if (ok)
            {
               std::ifstream f(paramFilename);
               if (f)
                  f.close();

               else
               {
                  msgError = "Bad argument: " + text;
                  ok = false;
               }
            }
         }
      }
      else if (text.length() > 7 && text.substr(0, 7) == "-LOWER=")
      {
         std::string str = text.substr(7, text.length() - 7);
         realpaver::Interval x(str.c_str());
         if (!x.isEmpty())
            objBounds.setLeft(x.left());
         else
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
      }
      else if (text.length() > 7 && text.substr(0, 7) == "-UPPER=")
      {
         std::string str = text.substr(7, text.length() - 7);
         realpaver::Interval x(str.c_str());
         if (!x.isEmpty())
            objBounds.setRight(x.right());
         else
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
      }
      else if (text.length() > 2 && text.substr(0, 2) == "-L")
      {
         if (logFilename != "")
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
         else
         {
            std::string str = text.substr(2, text.length() - 2);
            ok = expandTildeInPath(str, logFilename, msgError);

            if (ok)
            {
               std::ofstream of;
               of.open(logFilename, std::ofstream::out);
               if (of.bad())
               {
                  msgError = "Open error of log file";
                  ok = false;
               }
               else
               {
                  of.close();
               }
            }
         }
      }
      else if (text.length() > 2 && text.substr(0, 2) == "-O")
      {
         if (solFilename != "")
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
         else
         {
            std::string str = text.substr(2, text.length() - 2);
            ok = expandTildeInPath(str, solFilename, msgError);

            if (ok)
            {
               std::ofstream of;
               of.open(solFilename, std::ofstream::out);
               if (of.bad())
               {
                  msgError = "Open error of solution file";
                  ok = false;
               }
               else
               {
                  of.close();
               }
            }
         }
      }
      else if (text.length() > 2 && text.substr(0, 2) == "-D")
      {
         options.push_back(text.substr(2, text.length() - 2));
      }
      else
      {
         if (pbFilename != "")
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
         else
         {
            pbFilename = text;
            std::ifstream f(pbFilename);
            if (f)
               f.close();

            else
            {
               msgError = "Bad argument: " + text;
               ok = false;
            }
         }
      }
      i++;
   }

   if (pbFilename == "")
   {
      msgError = "No problem file!";
      ok = false;
   }

   return ok;
}

void parseFilename(const std::string &filename, std::string &path, std::string &base,
                   std::string &ext)
{
   bool iter = true, found = false;
   size_t pos = 0;
   do
   {
      size_t k = filename.find('/', pos);
      if (k == std::string::npos)
      {
         k = filename.find('\\', pos);
      }
      if (k == std::string::npos)
      {
         iter = false;
      }
      else
      {
         pos = k + 1;
         found = true;
      }
   } while (iter);

   if (found)
   {
      path = filename.substr(0, pos);
   }
   else
   {
      path = "";
   }

   size_t k = filename.find('.', pos);
   if (k == std::string::npos)
   {
      base = filename.substr(pos, filename.length() - pos);
      ext = "";
   }
   else
   {
      base = filename.substr(pos, k - pos);
      ext = filename.substr(k, filename.length() - k);
   }
}

bool processOptions(realpaver::Params &params, std::vector<std::string> &options,
                    std::string &msgError)
{
   for (const std::string &opt : options)
   {
      if (!params.readParam(opt))
      {
         msgError = "Bad option: " + opt;
         return false;
      }
   }
   return true;
}

bool expandTildeInPath(const std::string &path, std::string &res, std::string &msgError)
{
   res = "";
   for (char c : path)
   {
      if (c == '~')
      {
         const char *home = std::getenv("HOME");
         if (home)
         {
            res += std::string(home);
         }
         else
         {
            msgError = "Not able to replace ~ by HOME in " + path;
            return false;
         }
      }
      else
      {
         res += std::string(1, c);
      }
   }
   return true;
}

std::string WP(const std::string &s, int n)
{
   std::string str = s;
   int m = n - s.length();
   while (--m >= 0)
      str += ".";
   str += " ";
   return str;
}
