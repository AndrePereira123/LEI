<script setup>
import { ref, onMounted } from 'vue';
import { useAuthStore } from '../stores/auth'

const props = defineProps({
  studentId: {
    type: String,
    required: true
  },
  title: {
    type: String,
    required: true
  },
  path_horario_publicado: {
    type: String,
    default: ''
  }
});

const authStore = useAuthStore()

const schedule = ref({});
const days = ['Segunda-feira', 'Terça-feira', 'Quarta-feira', 'Quinta-feira', 'Sexta-feira'];
const hours = ['08:00', '09:00', '10:00', '11:00', '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00', '19:00'];
const studentId = props.studentId;
const path_horario_publicado = props.path_horario_publicado

const countInstances = (day, hour) => {
                  if (!schedule.value[day] || !schedule.value[day][hour]) {
                    return 0;
                  }
                  return schedule.value[day][hour].length;
                };

const isSlotOccupied = (day, hour) => {
  if (!schedule.value[day]) {
    return false;
  }

  const startHours = Object.keys(schedule.value[day]);
  const currentIndex = hours.indexOf(hour);

  for (let i = 0; i < startHours.length; i++) {
    const startHour = startHours[i];
    const slots = schedule.value[day][startHour].slots;
    const startIndex = hours.indexOf(startHour);

    if (currentIndex > startIndex && currentIndex < startIndex + slots) { // Verifica se a hora atual está dentro do intervalo de slots ocupados
      return true;
    }
  }

  return false;
};

onMounted(async () => {
  try {
    let allocations;
    
    if (path_horario_publicado === '') {
      
      const response = await fetch(`http://localhost:3000/allocations?studentId=${studentId}`);
      if (!response.ok) {
        throw new Error('Failed to fetch allocations');
      }
      allocations = await response.json();
    } 
    else {
      
      const publishedHorarioResponse = await fetch('http://localhost:3000/publishedHorarios/1');
      if (!publishedHorarioResponse.ok) {
        throw new Error('Failed to fetch published horarios');
      }
      
      
      const publishedHorario = await publishedHorarioResponse.json();
      
      
      allocations = publishedHorario.allocations.filter(
        allocation => String(allocation.studentId) === String(studentId)
      );
      
      
      if (!allocations || allocations.length === 0) {
        console.warn(`No allocations found for student ${studentId} in published horarios`);
        schedule.value = {};
        return;
      }
    }
    
    console.log("Filtered allocations:", allocations);
    
    const organizedSchedule = {};
    for (const allocation of allocations) {
      let shift;
      if (path_horario_publicado === '') {
        const shiftResponse = await fetch(`http://localhost:3000/shifts/${allocation.shiftId}`);    
        if (!shiftResponse.ok) {
          throw new Error('Failed to fetch shift details');
        }
        
        const shiftData = await shiftResponse.json();
        shift = Array.isArray(shiftData) ? shiftData[0] : shiftData;
      } else {
        const publishedResponse = await fetch('http://localhost:3000/publishedHorarios/1');
        if (!publishedResponse.ok) {
          throw new Error('Failed to fetch published horarios');
        }

        const publishedData = await publishedResponse.json();
        
        const foundShifts = publishedData.shifts.filter(s => 
          String(s.id) === String(allocation.shiftId)
        );
        
        if (!foundShifts || foundShifts.length === 0) {
          throw new Error(`Shift with ID ${allocation.shiftId} not found in published data`);
        }
        
        shift = foundShifts[0];
      }

      let classResponse;
      classResponse = await fetch(`http://localhost:3000/classrooms/${shift.classroomId}`);
      if (!classResponse.ok) {
        throw new Error('Failed to fetch classroom details');
      }
      const classroomData = await classResponse.json();
      const classroom = Array.isArray(classroomData) ? classroomData[0] : classroomData;

      let buildingResponse;
      buildingResponse = await fetch(`http://localhost:3000/buildings/${classroom.buildingId}`);
      if (!buildingResponse.ok) {
        throw new Error('Failed to fetch building details');
      }
      const buildingData = await buildingResponse.json();
      const building = Array.isArray(buildingData) ? buildingData[0] : buildingData;

      
      let courseResponse;
      courseResponse = await fetch(`http://localhost:3000/courses/${shift.courseId}`);
      
      if (!courseResponse.ok) {
        throw new Error('Failed to fetch course details');
      }
      const courseData = await courseResponse.json();
      const course = Array.isArray(courseData) ? courseData[0] : courseData;

      
      if (!organizedSchedule[shift.day]) {
        organizedSchedule[shift.day] = {};
      }

      const duration = parseInt(shift.to.split(':')[0]) - parseInt(shift.from.split(':')[0]);
      
      if(!organizedSchedule[shift.day][shift.from]) {
        organizedSchedule[shift.day][shift.from] = []
      }

      const isConflict = checkForConflicts(shift, organizedSchedule[shift.day][shift.from]);

      if (isConflict) {
        organizedSchedule[shift.day][shift.from].forEach((existingShift) => {
          existingShift.isConflict = true;
        });
      }

      organizedSchedule[shift.day][shift.from].push({ 
        name: `${course.abbreviation} ${shift.name}`, 
        location: `${building.abbreviation} - ${classroom.name}`, 
        duration: duration,
        slots: duration,
        isConflict: isConflict,
        from: shift.from,
        to: shift.to
      });
    }

    schedule.value = organizedSchedule;
  } catch (error) {
    console.error('Erro ao carregar o horário:', error);
  }
});

const checkForConflicts = (shift, existingShifts) => {
  const parseTime = (time) => {
    if (!time || typeof time !== 'string') {
      console.error('Invalid time:', time);
      return NaN;
    }
    const [hours, minutes] = time.split(':').map(Number);
    return hours * 60 + minutes;
  };

  const shiftStart = parseTime(shift.from);
  const shiftEnd = parseTime(shift.to);

  return existingShifts.some((existingShift) => {
    const existingStart = parseTime(existingShift.from);
    const existingEnd = parseTime(existingShift.to);

    return shiftStart < existingEnd && shiftEnd > existingStart;
  });
};
</script>

<template>
  <main class="schedule-view">
    <div class="Caixa_branca">
      <h1>{{ props.title }}</h1>
      <div class="schedule-grid">
        <!-- Time column with a placeholder for header alignment -->
        <div class="time-column">
          <div style="visibility: hidden; height: 5vh;">Placeholder</div>
          <div v-for="hour in hours" :key="hour" class="time-slot">
            <div class="time-ball">
              <p>{{ hour.split(':')[0] }}h</p>
            </div>
          </div>
        </div>

        <!-- Day columns -->
        <div class="day-column" v-for="day in days" :key="day">
          <h3>{{ day }}</h3>
          <div v-for="hour in hours" :key="hour" class="hour-slot">
            <!-- Slot com aulas que começam nesta hora -->
            <div
              v-if="schedule[day] && schedule[day][hour]"
              class="class-container"
            >
              <div
              v-for="(classInfo, index) in schedule[day][hour]"
              :key="classInfo.name"
              :class="['class-slot',
                classInfo.isConflict ? 'conflict' : 'normal',
                classInfo.isConflict && index < countInstances(day, hour) - 1 ? 'with-divider' : ''
              ]"
                :style="{ 
                  width: `calc(100% / ${countInstances(day, hour) || 1})`,
                  height: classInfo.slots > 1 ? `calc(100% * ${classInfo.slots})` : '100%',
                  gridRow: `span ${classInfo.slots}`
                }"
              >
                <p>{{ classInfo.name }}</p>
                <p>{{ classInfo.location }}</p>
              </div>
            </div>
            <div
              v-else-if="!isSlotOccupied(day, hour)"
              class="empty-slot"
            ></div>
          </div>
        </div>
      </div>
    </div>
  </main>
</template>

<style scoped>
.schedule-view {
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #2c3e50;
  color: white;
  padding: 1.4%;
  height: 100%;
  width: 100%;
  border-radius: 30px;
}

.Caixa_branca {
  overflow: hidden;
  height: 100%;
  max-height: 100%;
  width: 100%;
}

h1 {
  margin-top: -3vh;
  font-size: 2.5em;
  font-weight: bold;
  margin-bottom: 40px;
  color: #333e4f;
}
.class-slot.conflict {
  border-radius: 2px;
}

.class-slot.conflict.with-divider {
  border-right: 2px solid white;
}

.schedule-grid {
  display: grid;
  grid-template-columns: 14vh repeat(5, 1fr);
  gap: 0.7%;
  width: 100%;
  height: 100%;
  max-height: 100%;
  align-items: stretch;
  margin-top: -5vh;
  margin-left: -10vh;
  overflow: auto;
}

.day-column, .time-column {
  display: grid;
  grid-template-rows: 5vh repeat(12, 1fr);
  gap: 1%;
  height: 100%;
  width: 100%;
}

.day-column h3 {
  text-align: center;
  margin: 0;
  height: 100%;
  display: flex;
  align-items: center; 
  justify-content: center; 
  font-size: 1.2em;
  background-color: #2c3e50; 
  color: white;
}

.time-column {
  padding: 10px;
  color: white;
}

.time-slot {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 90%;
  width: 70%;
}

.time-ball {
  height: 80%;
  margin-left: 10vh;
}


.hour-slot {
  height: 100%;
  width: 100%;
  font-size: 1.3vh;
}

.class-container {
  display: flex;
  flex-direction: row;
  width: 100%;
  height: 100%;
}

.class-slot {
  color: white;
  border-radius: 5px;
  padding: 5px;
  text-align: left;
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
  overflow: hidden;
  position: relative;
  z-index: 2;
  height: 100%;
  width: 100%;
}

.class-slot p {
  margin: 0;
  font-size: 1.2em;
  white-space: nowrap;
  overflow: visible;
  text-overflow: ellipsis;
}

</style>

