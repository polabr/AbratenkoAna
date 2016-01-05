from ROOT import fememu
def apply_config(cfg_obj,cfg_file):

    if not cfg_file.endswith('.cfg'):
        print '\033[95m[ERROR]\033[00m config file (%s) must have a suffix .cfg' % cfg_file
        return False

    contents = open(cfg_file,'r').read().split('\n')
    for line_num in xrange(len(contents)):

        line = contents[line_num]

        if line.find('#')>=0: line = line[0:line.find('#')]
            
        words = line.split(':')

        if not words or (len(words)==1 and not words[0].strip(' ')): continue

        if not len(words) == 2:
            print '\033[93m[WARNING]\033[00m could not interpret following line (skipping)'
            print '    Line %d: %s' % (line_num,line)
            continue

        var_name = words[0].strip(' ')
        val_name = words[1].strip(' ')

        try:
            exec('cfg_obj.%s' % var_name) # this line checks if an attrivute exists w/ valid name
            exec('cfg_obj.%s = %s' % (var_name,val_name))
        except Exception:
            print '\033[95m[ERROR]\033[00m could not apply following line for configuration (skipping)'
            print '    Line %d: %s' % (line_num,line)
            return False

    return True
