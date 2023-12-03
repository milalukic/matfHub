#include "../include/course.h"
#include <set>
#include <string>

Course::Course(
    const std::string& desc,
    int d,
    const std::string& t,
    int s,
    int dur,
    const std::string& type,
    const std::set<std::string>& grps,
    std::string& clsrm,
    const std::set<std::string>& mods,
    const std::set<int>& yrs,
    const std::set<std::string>& subgrps
    ) : description(desc), day(d), teacher(t), start(s), duration(dur),
    end(s + dur), course_type(type), groups(grps), classroom(clsrm),
    modules(mods), years(yrs), subgroups(subgrps) {}
