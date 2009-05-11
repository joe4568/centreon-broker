/*
** hoststatusevent.h for CentreonBroker in ./inc
** 
** Made by Matthieu Kermagoret <mkermagoret@merethis.com>
** 
** Copyright Merethis
** See LICENSE file for details.
** 
** Started on  05/07/09 Matthieu Kermagoret
** Last update 05/11/09 Matthieu Kermagoret
*/

#ifndef HOSTSTATUSEVENT_H_
# define HOSTSTATUSEVENT_H_

# include <string>
# include "event.h"

namespace            CentreonBroker
{
  class              EventSubscriber;

  class              HostStatusEvent : public Event
  {
   private:
    int              hoststatus_id;
    short            instance_id;
    int              host_object_id;
    time_t           status_update_time;
    std::string      output;
    std::string      perfdata;
    short            current_state;
    short            has_been_checked;
    short            should_be_scheduled;
    short            current_check_attempt;
    short            max_check_attempts;
    time_t           last_check;
    time_t           next_check;
    short            check_type;
    time_t           last_state_change;
    time_t           last_hard_state_change;
    time_t           last_time_up;
    time_t           last_time_down;
    time_t           last_time_unreachable;
    short            state_type;
    time_t           last_notification;
    time_t           next_notification;
    short            no_more_notifications;
    short            notifications_enabled;
    short            problem_has_been_acknowledged;
    short            acknowledgement_type;
    short            current_notification_number;
    short            passive_checks_enabled;
    short            active_checks_enabled;
    short            event_handler_enabled;
    short            flap_detection_enabled;
    short            is_flapping;
    double           percent_state_change;
    double           latency;
    double           execution_time;
    short            scheduled_downtime_depth;
    short            failure_prediction_enabled;
    short            process_performance_data;
    short            obsess_over_host;
    int              modified_host_attributes;
    std::string      event_handler;
    std::string      check_command;
    double           normal_check_interval;
    double           retry_check_interval;
    int              check_timeperiod_object_id;

   public:
                     HostStatusEvent();
                     HostStatusEvent(const HostStatusEvent& hse);
                     ~HostStatusEvent();
    HostStatusEvent& operator=(const HostStatusEvent& hse);
    void             AcceptVisitor(EventSubscriber& es);
    int              GetType() const;
    // Setters
    void             SetHostStatusId(int hsi);
    void             SetInstanceId(short ii);
    void             SetHostObjectId(int hoi);
    void             SetStatusUpdateTime(time_t sut);
    void             SetOutput(const std::string& o);
    void             SetPerfdata(const std::string& p);
    void             SetCurrentState(short cs);
    void             SetHasBeenChecked(short hbc);
    void             SetShouldBeScheduled(short sbs);
    void             SetCurrentCheckAttempt(short cca);
    void             SetMaxCheckAttempts(short mca);
    void             SetLastCheck(time_t lc);
    void             SetNextCheck(time_t nc);
    void             SetCheckType(short ct);
    void             SetLastStateChange(time_t lsc);
    void             SetLastHardStateChange(time_t lhsc);
    void             SetLastTimeUp(time_t ltu);
    void             SetLastTimeDown(time_t ltd);
    void             SetLastTimeUnreachable(time_t ltu);
    void             SetStateType(short st);
    void             SetLastNotification(time_t ln);
    void             SetNextNotification(time_t nn);
    void             SetNoMoreNotifications(short nmn);
    void             SetNotificationsEnabled(short ne);
    void             SetProblemHasBeenAcknowledged(short phba);
    void             SetAcknowledgementType(short at);
    void             SetCurrentNotificationNumber(short cnn);
    void             SetPassiveChecksEnabled(short pce);
    void             SetActiveChecksEnabled(short ace);
    void             SetEventHandlerEnabled(short ehe);
    void             SetFlapDetectionEnabled(short fde);
    void             SetIsFlapping(short i_f);
    void             SetPercentStateChange(double psc);
    void             SetLatency(double l);
    void             SetExecutionTime(double et);
    void             SetScheduledDowntimeDepth(short sdd);
    void             SetFailurePredictionEnabled(short fpe);
    void             SetProcessPerformanceData(short ppd);
    void             SetObsessOverHost(short ooh);
    void             SetModifiedHostAttributes(int mha);
    void             SetEventHandler(const std::string& eh);
    void             SetCheckCommand(const std::string& cc);
    void             SetNormalCheckInterval(double nci);
    void             SetRetryCheckInterval(double rci);
    void             SetCheckTimeperiodObjectId(int ctoi);
  };
}

#endif /* !HOSTSTATUSEVENT_H_ */
