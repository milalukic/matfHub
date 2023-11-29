#include "../include/course.h"
#include <set>
#include <string>

Course::Course(
    const std::string& desc,
    const std::string& d,
    const std::string& t,
    int s,
    int dur,
    const std::string& type,
    const std::set<std::string>& grps,
    const std::string& clsrm,
    const std::set<std::string>& mods,
    int yrs,
    const std::set<std::string>& subgrps
    ) : description(desc), day(d), teacher(t), start(s), duration(dur),
    end(s + dur), course_type(type), groups(grps), classroom(clsrm),
    modules(mods), years(yrs), subgroups(subgrps) {}
