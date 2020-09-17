#!/bin/sh

source ./xagent.$1.env || exit 1

/etc/init.d/XAgent stop

d2 -c xagentcfg[0].x_register_url  "${XAGENT_REGISTER_URL}"
d2 -c xagentcfg[0].x_claimed_url   "${XAGENT_CLAIMED_URL}"
d2 -c xagentcfg[0].x_discovery_url "${XAGENT_DISCOVERY_URL}"
d2 -c xagentcfg[0].x_advisor_url   "${XAGENT_ADVISOR_URL}"

d2 -c xagentcfg[0].x_agent_id         ""
d2 -c xagentcfg[0].x_agent_claim_code ""
d2 -c xagentcfg[0].x_agent_registered		0
d2 -c xagentcfg[0].x_agent_claimed		0

d2 -c xagentstatus[0].Registered 0
/etc/init.d/XAgent start

# wait for re-registration
# d2 -w xagentstatus[0].Registered
sleep 5
