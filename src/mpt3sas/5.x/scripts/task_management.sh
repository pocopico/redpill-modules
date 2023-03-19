# DISCLAIMER OF LIABILITY
# THIS IS SAMPLE SCRIPT. 
# NEITHER RECIPIENT NOR ANY CONTRIBUTORS SHALL HAVE ANY LIABILITY FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING WITHOUT LIMITATION LOST PROFITS), HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OR DISTRIBUTION OF THE PROGRAM OR THE EXERCISE OF ANY RIGHTS GRANTED
# HEREUNDER, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES

#!/bin/bash

DR=1
MUR=2
TASKTYPE_ABORT_TASK=3
TASKTYPE_TARGET_RESET=4
TASKTYPE_LOGICAL_UNIT_RESET=5
TASKTYPE_ABRT_TASK_SET=6

dmesg -c > /dev/null
clear
scsi_host="/sys/class/scsi_host"
cd ${scsi_host}
subfolders=`ls -1`
for i in ${subfolders};  do
	cd ${i};
	if [ `cat proc_name` == "mpt3sas" ]; then
#		echo ${TASKTYPE_ABORT_TASK} > task_management
		echo ${TASKTYPE_TARGET_RESET} > task_management
#		echo ${TASKTYPE_LOGICAL_UNIT_RESET} > task_management
#		echo ${TASKTYPE_ABRT_TASK_SET} > task_management
#		echo ${MUR} > task_management
#		echo ${DR} > task_management
	fi;
	cd ${scsi_host}
done;
echo -e \\n
dmesg
