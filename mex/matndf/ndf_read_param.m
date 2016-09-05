function pvalue = ndf_read_param(pfile, ptype, ptask, pname)
% pvalue = ndf_read_param(pfile, ptype, ptask, pname)
%
% The function reads a formatted string parameter from PFILE (only last 
% line of the PFILE). String parameter is formatted as follows: 
%   
%   type|PTYPE|task|PTASK|PNAME|PVALUE
% 
% If the required PTYPE, PTASK or PNAME are not found, an empty string is
% returned.
%
% Example:
%
% Last line in PFILE:
% '[20160905143936] type|artifact|task|artifact_on|threshold|6.41'
%
% >> pvalue = ndf_read_param(PFILE, 'artifact', 'artifact_on', 'threshold');
% 
% pvalue = '6.41'

    pvalue = '';
    
    line = read_last_line(pfile);
    if (isempty(line) == 0)
        type  = regexp(line, 'type\|(?<value>[\w]+)', 'names');
        task  = regexp(line, 'task\|(?<value>[\w]+)', 'names');
        param = regexp(line, [pname '\|(?<value>([-+]?([0-9]*\.[0-9]+|[0-9]+)|\w+))'], 'names');

        if (isempty(type) || isempty(task) || isempty(param)) == 0
            if(strcmp(type.value, ptype) && strcmp(task.value, ptask))
                pvalue = param.value;
            end
        end
    end
end

function line = read_last_line(src)

    fid = fopen(src,'r');                               % Open the file as a binary
    line = '';                                          % Initialize to empty
    offset = 1;                                         % Offset from the end of file
    if(fid > 0)
        fseek(fid,-offset,'eof');                           % Seek to the file end, minus the offset
        newChar = fread(fid,1,'*char');                     % Read one character
        while (~strcmp(newChar,char(10))) || (offset == 1)
            line = cat(2, line, newChar);                   % Add the character to a string
            offset = offset+1;
            fseek(fid,-offset,'eof');                       % Seek to the file end, minus the offset
            newChar = fread(fid,1,'*char');                 % Read one character
        end
        fclose(fid);                                        % Close the file
        line = fliplr(line);
    end
end