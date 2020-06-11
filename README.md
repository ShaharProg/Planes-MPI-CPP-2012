# Planes
How the system works

AGENCY creates the "world" by a configuration file containing all the information about the world. The world includes some air space, aircraft and their flight routes at certain points. AGENCY begins to distribute the work to OPERTIVES equally, each OPERTIVE receives a certain amount of time and all information on the aircraft that is on time. OPERTIVE calculates the amount of concealment on the entire aircraft when you finish the job. In addition to the division of labor between processors each processor divides the work into its various cores through OMP.

CUDA The system to understand OPERTIVE has a war for the GPU for the information and all "heavy" calculations will be performed on the GPU more quickly and directly back to OPERTIVE.

------------------------------

דרך עבודת המערכת

המערכת מחולקת לMASTER ולכמות בלתי ידועה של SLAVES ע"י MPI באופן הבא.
ה-MASTER יוצר את "העולם" ע"י קובץ קונפיגורציה המכיל את כל המידע על העולם.
העולם כולל שטח אוירי מסויים, מטוסים ומסלולי הטיסה שלהם בנקודות מסויימות.
לאחר מכן הMASTER מתחיל לחלק את העבודה לSLAVE-ים בצורה שווה, כלומר כל SLAVE מקבל dt מסויים ואת כל המידע על המטוסים שנמצאים בזמן הנתון. והSLAVE מחשב את כמות ההסתרות כלפי כל מטוס, כשמסיים את העבודה הSLAVE מחזיר לMASTER את המידע על המטוסים והMASTER מעדכן את הנתונים הכלליים שנשמרים אצלו ומיד נותן לאותו SLAVE עבודה חדשה.
בנוסף לחלוקת העבודה בין המעבדים כל מעבד מחלק את העבודה לליבות השונות שלו בעזרת OMP.

הצעה לצירוף CUDA למערכת
שכל SLAVE ישלח לGPU את המידע וכל החישובים "הכבדים" יבוצעו על הGPU בצורה מהירה יותר ולאחר מכן יחזרו לSLAVE.
