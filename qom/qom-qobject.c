/*
 * QEMU Object Model - QObject wrappers
 *
 * Copyright (C) 2012 Red Hat, Inc.
 *
 * Author: Paolo Bonzini <pbonzini@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu-common.h"
#include "qom/object.h"
#include "qom/qom-qobject.h"
#include "qapi/visitor.h"
#include "qapi/qmp-input-visitor.h"
#include "qapi/qmp-output-visitor.h"

#if defined(CONFIG_GNU_ARM_ECLIPSE)
#include "qapi/qmp/types.h"
#include "qemu/log.h"
#endif

#if defined(CONFIG_GNU_ARM_ECLIPSE__)
static char *dump_value(QObject *value, char *buf, size_t siz)
{
    if (value->type->code == QTYPE_QINT) {
        QInt *p = (QInt *) value;
        snprintf(buf, siz, "%" PRId64, p->value);
        return buf;
    } else if (value->type->code == QTYPE_QSTRING) {
        QString *p = (QString *) value;
        return p->string;
    } else if (value->type->code == QTYPE_QBOOL) {
        QBool *p = (QBool *) value;
        return (char*) (p->value ? "true" : "false");
    }
    return (char*) "?";
}
#endif

void object_property_set_qobject(Object *obj, QObject *value,
                                 const char *name, Error **errp)
{
    QmpInputVisitor *qiv;
    qiv = qmp_input_visitor_new(value);
    object_property_set(obj, qmp_input_get_visitor(qiv), name, errp);

    qmp_input_visitor_cleanup(qiv);
}

QObject *object_property_get_qobject(Object *obj, const char *name,
                                     Error **errp)
{
    QObject *ret = NULL;
    Error *local_err = NULL;
    QmpOutputVisitor *qov;

    qov = qmp_output_visitor_new();
    object_property_get(obj, qmp_output_get_visitor(qov), name, &local_err);
    if (!local_err) {
        ret = qmp_output_get_qobject(qov);
    }
    error_propagate(errp, local_err);
    qmp_output_visitor_cleanup(qov);
    return ret;
}
